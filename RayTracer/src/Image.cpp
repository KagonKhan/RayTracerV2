#include "Image.hpp"

#include <GLFW/glfw3.h>

Image::Image(Vec2i size) : size(size) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Image::setData(const int *data) {
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Image::resize(Vec2i newSize) {
  if (size == newSize)
    return;

  size = newSize;

  // TODO: later some optimizations possible on GL side?
}
