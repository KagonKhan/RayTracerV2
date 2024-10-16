#include "Renderer.hpp"

#include "RayMath/Point.hpp"
#include "RayMath/Vector.hpp"

void Renderer::onResize(Vec2i newSize) {
  if (newSize != image->getSize()) {
    image->resize(newSize);

    delete imageData;
    imageData = new int[newSize.x * newSize.y];
  }
}

std::shared_ptr<Image> Renderer::render() {
  Vec2f position{0.f, 0.f};

  const auto size = image->getSize();
  for (int y = 0; y < size.y; ++y) {
    position.y = (float)y / (float)size.y;
    for (int x = 0; x < size.x; ++x) {
      position.x = (float)x / (float)size.x;
      auto index = y * size.x + x;
      imageData[index] = perPixel(position);
    }
  }

  image->setData(imageData);
  return image;
}

RayMath::Point rayOrigin(0.f, 0.f, -1.f);
RayMath::Vector sphereToRay = rayOrigin - RayMath::Point{};
RayMath::Vector rayDirection(0, 0, -1.f);

#include <iostream>

int Renderer::perPixel(Vec2f position) {
  rayDirection.x = position.x;
  rayDirection.y = position.y;

  const float a = rayDirection.dot(rayDirection);
  const float b = 2.f * rayDirection.dot(sphereToRay);
  const float c = sphereToRay.dot(sphereToRay) - 1.f;
  const float d = b * b - 4.f * a * c;
  if (d >= 0.f) {
    return 0xffff00ff;
  } else
    return 0xff0ff000;
}