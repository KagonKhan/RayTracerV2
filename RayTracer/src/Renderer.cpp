#include "Renderer.hpp"

#include <algorithm>
#include <execution>


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
    }
}

void Renderer::render (Camera const &camera, Scene const &scene) {
    this->camera    = &camera;
    this->scene     = &scene;
    const auto size = image->getSize ();

    if (m_FrameIndex == 1)
        memset (m_AccumulationData, 0, image->getSize ().x * image->getSize ().y * sizeof (glm::vec4));


    std::for_each (std::execution::seq, m_ImageVerticalIter.begin (), m_ImageVerticalIter.end (), [this] (int y) {
        std::for_each (std::execution::seq, m_ImageHorizontalIter.begin (), m_ImageHorizontalIter.end (), [this, y] (int x) {
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
    glm::vec3 contribution (1.0f);

    for (int i = 0; i < settings.bounces; i++) {
        Renderer::HitPayload payload = TraceRay (ray);
        if (payload.HitDistance < 0.0f) {
            glm::vec3 skyColor = glm::vec3 (0.6f, 0.7f, 0.9f);
            // light += skyColor * contribution;
            break;
        }

        const Sphere   &sphere   = scene->spheres[payload.ObjectIndex];
        const Material &material = scene->materials[sphere.materialIndex];

        contribution *= material.Albedo;
        light += material.GetEmission ();

        ray.origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
        // ray.direction = glm::reflect (ray.direction, payload.WorldNormal + material.Roughness * Random::Vec3 (-0.5f, 0.5f));
        ray.direction = glm::normalize (payload.WorldNormal + Random::InUnitSphere ());
    }

    return glm::vec4 (light, 1.0f);
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
        return Miss (ray);

    return ClosestHit (ray, hitDistance, closestSphere);
}

Renderer::HitPayload Renderer::ClosestHit (const Ray &ray, float hitDistance, int objectIndex) {
    Renderer::HitPayload payload;
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = objectIndex;

    const Sphere &closestSphere = scene->spheres[objectIndex];

    glm::vec3 origin      = ray.origin - closestSphere.position;
    payload.WorldPosition = origin + ray.direction * hitDistance;
    payload.WorldNormal   = glm::normalize (payload.WorldPosition);

    payload.WorldPosition += closestSphere.position;

    return payload;
}

Renderer::HitPayload Renderer::Miss (const Ray &ray) {
    Renderer::HitPayload payload;
    payload.HitDistance = -1.0f;
    return payload;
}