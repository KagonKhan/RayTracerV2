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
        Sphere s1{{0, 0, -1}, 0.5f};
        Sphere s2{{50, 25.5, -75}, 20.f};
        w.add (std::make_shared<Sphere> (s1));
        w.add (std::make_shared<Sphere> (s2));
        once = false;
    }
}

// Camera

auto            focal_length    = 1.0;
auto            viewport_height = 2.0;
auto            camera_center   = RayMath::Point ();
auto            viewport_v      = RayMath::Vector (0, -viewport_height, 0);
float           viewport_width  = 0.f;
RayMath::Vector viewport_u;
RayMath::Vector pixel_delta_u;
RayMath::Vector pixel_delta_v;
RayMath::Point  viewport_upper_left;
RayMath::Point  pixel00_loc;

std::shared_ptr<Image> Renderer::render (Camera const &camera) {
    const auto size = image->getSize ();

    viewport_width      = viewport_height * (float (size.x) / size.y);
    viewport_u          = RayMath::Vector (viewport_width, 0, 0);
    pixel_delta_u       = viewport_u / (float) size.x;
    pixel_delta_v       = viewport_v / (float) size.y;
    viewport_upper_left = camera_center - RayMath::Vector (0, 0, focal_length) - viewport_u / 2 - viewport_v / 2.f;
    pixel00_loc         = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5f;


    std::for_each (std::execution::seq, vertIter.begin (), vertIter.end (), [this] (int y) {
        std::for_each (std::execution::seq, horIter.begin (), horIter.end (), [this, y] (int x) {
            imageData[y * this->image->getSize ().x + x] = perPixel ({x, y}).toRGBA ();
        });
    });


    image->setData (imageData);
    return image;
}

const RayMath::Vector rayOrigin{-1.f, -0.5f, 1.f};
RayMath::Vector       rayDirection{0.f, 0.f, -1.f};
const float           radius{0.5f};


RayMath::Color Renderer::perPixel (Vec2i position) {
    const auto         pixel_center  = pixel00_loc + (pixel_delta_u * position.x) + (pixel_delta_v * position.y);
    const auto         ray_direction = pixel_center - camera_center;
    const RayMath::Ray r{camera_center, ray_direction};


    if (w.hit (r, 0, 99999999999)) {
        return RayMath::Colors::Cyan;
    }


    RayMath::Vector unit_direction = r.direction.normalized ();

    float a = 0.5f * (unit_direction.y + 1.f);
    return RayMath::Color (255.f, 255.f, 255.f, 255.f) * (1.f - a) + RayMath::Color (255 * 0.5f, 255 * 0.7f, 255 * 1.f, 255.f) * a;
}
