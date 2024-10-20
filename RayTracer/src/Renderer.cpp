#include "RayMath/Color.hpp"
#include "RayMath/Point.hpp"
#include "RayMath/Ray.hpp"
#include "RayMath/Vector.hpp"
#include "Renderer.hpp"

#include <algorithm>
#include <execution>


void Renderer::onResize (Vec2i newSize) {
    if (newSize != image->getSize ()) {
        image->resize (newSize);

        delete imageData;
        imageData = new int[newSize.x * newSize.y];

        horIter.resize (newSize.x);
        vertIter.resize (newSize.y);

        for (uint32_t i = 0; i < newSize.x; i++)
            horIter[i] = i;
        for (uint32_t i = 0; i < newSize.y; i++)
            vertIter[i] = i;
    }


    if (static bool once = true; once) {
        Sphere s1{};
        s1.setTransform (RayMath::Matrix{1.f}.Scaled (0.33, 0.33, 0.33));
        s1.material.color        = RayMath::Color (255, 0, 0);
        s1.material.ambient      = 0.5f;
        s1.material.diffuse      = 0.5f;
        s1.material.specular     = 0.9f;
        s1.material.shininess    = 300.f;
        s1.material.reflective   = 0.9;
        s1.material.transparency = 0.9;
        s1.material.refraction   = 5;

        Sphere s2{};
        s2.setTransform (RayMath::Matrix{1.f}.translated (-3, 0, 0).Scaled (0.25, 0.25, 0.25));
        s2.material.color        = RayMath::Color (0, 255, 255);
        s2.material.ambient      = 0.5f;
        s2.material.diffuse      = 0.5f;
        s2.material.specular     = 0.9f;
        s2.material.shininess    = 300.f;
        s2.material.reflective   = 0.9;
        s2.material.transparency = 0.9;
        s2.material.refraction   = 0.5;
        w.add (std::make_shared<Sphere> (s1));
        w.add (std::make_shared<Sphere> (s2));
        w.add (std::make_shared<PointLight> (RayMath::Point (0, -8, 0), RayMath::Color (0.9, 0.9, 0.9)));
        once = false;
    }
}

std::shared_ptr<Image> Renderer::render (Camera const &camera) {
    this->camera    = &camera;
    const auto size = image->getSize ();

    std::for_each (std::execution::seq, vertIter.begin (), vertIter.end (), [this] (int y) {
        std::for_each (std::execution::seq, horIter.begin (), horIter.end (), [this, y] (int x) {
            imageData[y * this->image->getSize ().x + x] = perPixel ({x, y}).toRGBA ();
        });
    });


    image->setData (imageData);
    return image;
}
Intersection getPositive (std::vector<Intersection> &xs) {
    std::ranges::sort (xs, {}, &Intersection::t);
    for (auto x : xs | std::views::filter ([] (Intersection const &x) { return x.t > 0.f; })) {
        return x;
    }
    return {};
}

RayMath::Color Renderer::perPixel (Vec2i position) {
    RayMath::Ray ray;
    ray.origin    = camera->GetPosition ();
    ray.direction = camera->GetRayDirections ()[image->getSize ().x * position.y + position.x];
    auto xs       = w.hit (ray, 0, 99999999999);
    auto hit      = getPositive (xs);

    if (hit.obj != nullptr) {
        auto comps = hit.Compute (ray, xs);
        return w.Shading (comps, 1);
    }

    RayMath::Vector unit_direction = ray.direction.normalized ();

    float a = 0.5f * (unit_direction.y + 1.f);
    return RayMath::Color (255.f, 255.f, 255.f, 255.f) * (1.f - a) + RayMath::Color (255 * 0.5f, 255 * 0.7f, 255 * 1.f, 255.f) * a;
}
