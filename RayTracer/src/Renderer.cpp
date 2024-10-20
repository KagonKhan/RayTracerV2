#include "RayMath/Color.hpp"
#include "RayMath/Point.hpp"
#include "RayMath/Ray.hpp"
#include "RayMath/Vector.hpp"
#include "Renderer.hpp"

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
        Sphere s1{{0, 0, 5}, 0.5f};
        // Sphere s2{{50, 25.5, -75}, 20.f};
        w.add (std::make_shared<Sphere> (s1));
        // w.add (std::make_shared<Sphere> (s2));
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


RayMath::Color Renderer::perPixel (Vec2i position) {
    RayMath::Ray ray;
    ray.origin    = camera->GetPosition ();
    ray.direction = camera->GetRayDirections ()[position.y * image->getSize ().x + position.y];

    if (w.hit (ray, 0, 99999999999)) {
        return RayMath::Colors::Cyan;
    }


    RayMath::Vector unit_direction = ray.direction.normalized ();

    float a = 0.5f * (unit_direction.y + 1.f);
    return RayMath::Color (255.f, 255.f, 255.f, 255.f) * (1.f - a) + RayMath::Color (255 * 0.5f, 255 * 0.7f, 255 * 1.f, 255.f) * a;
}
