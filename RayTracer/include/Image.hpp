#pragma once
#include "Utils.hpp"

class Image {
public:
  Image(Vec2i size);

  void setData(const int *data);
  void resize(Vec2i newSize);

  unsigned int getID() const { return texture; }
  Vec2i getSize() const { return size; }

private:
  unsigned int texture;
  Vec2i size;
};
