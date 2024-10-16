#pragma once
#include "Camera.hpp"
#include "Image.hpp"
#include "RayObjects/IHitObject.hpp"
#include "Utils.hpp"

#include <vector>


namespace RayMath {
struct Color;
}
class Renderer {
  public:
    void                   onResize (Vec2i newSize);
    std::shared_ptr<Image> render (Camera const &camera);
    std::shared_ptr<Image> getImage () { return image; }

  private:
    RayMath::Color perPixel (Vec2i position);

    std::shared_ptr<Image> image{new Image ({0, 0})};
    int                   *imageData = nullptr;

    std::vector<int> horIter, vertIter;

    World w;
};
