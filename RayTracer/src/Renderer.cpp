#include "Renderer.hpp"

void Renderer::onResize(Vec2i newSize) {
  if (newSize != image->getSize()) {
    image->resize(newSize);

    delete imageData;
    imageData = new int[newSize.x * newSize.y];
  }
}

std::shared_ptr<Image> Renderer::render() {
  const auto size = image->getSize();
  for (int y = 0; y < size.y; ++y) {
    for (int x = 0; x < size.x; ++x) {
      auto index = y * size.x + x;
      imageData[index] = perPixel({x / (float)size.x, y / (float)size.y});
    }
  }

  image->setData(imageData);
  return image;
}

int Renderer::perPixel(Vec2f position) {
  auto r = (uint8_t)(position.x * 255.f);
  auto g = (uint8_t)(position.y * 255.f);

  return 0xff000000 | (g << 8) | r;
}