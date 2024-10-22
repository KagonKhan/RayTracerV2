#include "Renderer.hpp"

#include <algorithm>
#include <execution>


void Renderer::onResize (glm::vec2 newSize) {
    if (newSize != image->getSize ()) {
        image->resize (newSize);

        delete imageData;
        imageData = new int[newSize.x * newSize.y];

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

    std::for_each (std::execution::seq, m_ImageVerticalIter.begin (), m_ImageVerticalIter.end (), [this] (int y) {
        std::for_each (std::execution::seq, m_ImageHorizontalIter.begin (), m_ImageHorizontalIter.end (),
                       [this, y] (int x) { imageData[y * (int) this->image->getSize ().x + (int) x] = toRGBA (perPixel (x, y)); });
    });


    image->setData (imageData);
}

glm::vec4 Renderer::perPixel (int x, int y) {
    Ray ray;
    ray.origin    = camera->GetPosition ();
    ray.direction = camera->GetRayDirections ()[image->getSize ().x * y + x];

    return Colors::Cyan;
}
