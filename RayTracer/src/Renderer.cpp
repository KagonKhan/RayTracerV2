#include "Renderer.hpp"

#include <algorithm>
#include <execution>

const Renderer::HitPayload Renderer::HitPayload::Miss;

void Renderer::onResize (glm::uvec2 newSize) {
    if (newSize != image->getSize ()) {
        image->resize (newSize);

        delete[] imageData;
        imageData = new int[newSize.x * newSize.y];
        delete[] m_AccumulationData;
        m_AccumulationData = new glm::vec4[newSize.x * newSize.y];

        m_ImageHorizontalIter.resize (newSize.x);
        m_ImageVerticalIter.resize (newSize.y);

        for (uint32_t i = 0; i < newSize.x; i++)
            m_ImageHorizontalIter[i] = i;
        for (uint32_t i = 0; i < newSize.y; i++)
            m_ImageVerticalIter[i] = i;

        ResetFrameIndex ();
    }
}

void Renderer::render (Camera const &camera, Scene const &scene) {
    this->camera    = &camera;
    this->scene     = &scene;
    const auto size = image->getSize ();

    if (m_FrameIndex == 1)
        memset (m_AccumulationData, 0, image->getSize ().x * image->getSize ().y * sizeof (glm::vec4));


    std::for_each (std::execution::seq, m_ImageVerticalIter.begin (), m_ImageVerticalIter.end (), [this] (int y) {
        std::for_each (std::execution::seq, m_ImageHorizontalIter.begin (), m_ImageHorizontalIter.end (),
                       [this, y] (int x) {
                           auto      pixel_color = perPixel (x, y);
                           const int pixel_index = x + y * image->getSize ().x;
                           m_AccumulationData[pixel_index] += pixel_color;

                           glm::vec4 accumulatedColor = m_AccumulationData[x + y * image->getSize ().x];
                           accumulatedColor /= (float) m_FrameIndex;
                           accumulatedColor       = glm::clamp (accumulatedColor, glm::vec4 (0.0f), glm::vec4 (1.0f));
                           imageData[pixel_index] = toRGBA (accumulatedColor);
                       });
    });


    image->setData (imageData);

    if (settings.Accumulate)
        m_FrameIndex++;
    else
        m_FrameIndex = 1;
}

glm::vec4 Renderer::perPixel (int x, int y) {
    Ray ray;
    ray.origin    = camera->GetPosition ();
    ray.direction = camera->GetRayDirections ()[(int) image->getSize ().x * y + x];

    glm::vec3 light (0.0f);
    glm::vec3 contribution (0.9f);
    uint32_t  seed = x + y * image->getSize ().x * m_FrameIndex;

    for (int i = 0; i < settings.bounces; i++) {
        seed += i;
        Renderer::HitPayload payload = TraceRay (ray);

        if (payload.HitDistance < 0.0f) {
            glm::vec3 skyColor = glm::vec3 (0.6f, 0.7f, 0.9f);
            light += skyColor * contribution;
            break;
        }

        const Sphere   &sphere   = scene->spheres[payload.ObjectIndex];
        const Material &material = scene->materials[sphere.materialIndex];


        // this is similar to recursive calls;
        ray.origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
        if (material.Refractive > 0.f) {
            ray.origin          = payload.WorldPosition - payload.WorldNormal * 0.0001f;
            auto unit_direction = glm::normalize (ray.direction);
            ray.direction       = refraction (payload, unit_direction, seed);

        } else if (material.Metallic > 0.f) {
            contribution *= material.Albedo;
            contribution *= material.Metallic;
            auto reflected = glm ::reflect (ray.direction, payload.WorldNormal);
            ray.direction  = reflected + material.Roughness * Random::InUnitHemiSphere (seed, payload.WorldNormal);
        } else if (material.Refractive <= 0.0f && material.Metallic <= 0.0f) {
            contribution *= material.Albedo;
            ray.direction = glm::normalize (payload.WorldNormal + Random::InUnitHemiSphere (seed, payload.WorldNormal));
        }

        light += material.GetEmission ();
    }

    return glm::vec4 (light, 1.0f);
}


float reflectance (double cosine, float refraction_index) {
    auto r0 = (1.0f - refraction_index) / (1.0f + refraction_index);
    r0 *= r0;
    return r0 + (1.0f - r0) * std::powf ((1.0f - cosine), 5.0f);
}

glm::vec3 Renderer::refraction (Renderer::HitPayload const &payload, glm::vec3 const &rayDir, uint32_t &seed) {
    const Sphere   &sphere     = scene->spheres[payload.ObjectIndex];
    const Material &material   = scene->materials[sphere.materialIndex];
    const auto      out_normal = glm::normalize (payload.WorldPosition - sphere.position);
    const bool      front_face = glm::dot (rayDir, out_normal) < 0;

    float     refractive_index = material.Refractive;
    glm::vec3 worldNormal;

    if (!front_face) {
        worldNormal = -payload.WorldNormal;
    } else {
        worldNormal      = payload.WorldNormal;
        refractive_index = 1.0f / material.Refractive;
    }


    const float cos_theta = glm::dot (-rayDir, worldNormal);
    const float sin_theta = std::sqrtf (1.0f - cos_theta * cos_theta);

    bool cannot_refract = refractive_index * sin_theta > 1.0f;
    if (cannot_refract || reflectance (cos_theta, refractive_index) > Random::RandomFloatPCG (seed)) {
        auto reflected = glm ::reflect (rayDir, worldNormal);
        return reflected + material.Roughness * Random::InUnitHemiSphere (seed, worldNormal);
    } else {
        return glm::refract (rayDir, worldNormal, refractive_index);
    }
}


Renderer::HitPayload Renderer::TraceRay (const Ray &ray) {
    int   closestSphere = -1;
    float hitDistance   = std::numeric_limits<float>::max ();

    for (size_t i = 0; i < scene->spheres.size (); i++) {
        const Sphere &sphere = scene->spheres[i];
        glm::vec3     origin = ray.origin - sphere.position;

        float a = glm::dot (ray.direction, ray.direction);
        float b = 2.0f * glm::dot (origin, ray.direction);
        float c = glm::dot (origin, origin) - sphere.radius * sphere.radius;

        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f)
            continue;


        // float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a); // Second hit distance (currently unused)
        float closestT = (-b - glm::sqrt (discriminant)) / (2.0f * a);
        if (closestT > 0.0f && closestT < hitDistance) {
            hitDistance   = closestT;
            closestSphere = (int) i;
        }
    }

    if (closestSphere < 0)
        return HitPayload::Miss;

    return ClosestHit (ray, hitDistance, closestSphere);
}

Renderer::HitPayload Renderer::ClosestHit (const Ray &ray, float hitDistance, int objectIndex) {
    Renderer::HitPayload payload;
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = objectIndex;

    const Sphere &closestSphere = scene->spheres[objectIndex];

    const glm::vec3 origin = ray.origin - closestSphere.position;
    payload.WorldPosition  = origin + ray.direction * hitDistance;
    payload.WorldNormal    = glm::normalize (payload.WorldPosition);

    payload.WorldPosition += closestSphere.position;

    return payload;
}
