#pragma once
#include "Image.hpp"
#include "Utils.hpp"

class Renderer {
public:
  void onResize(Vec2i newSize);
  std::shared_ptr<Image> render();

private:
  int perPixel(Vec2f position);

  std::shared_ptr<Image> image{new Image({0, 0})};
  int *imageData = nullptr;
};
