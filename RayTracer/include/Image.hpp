#pragma once
#include "Utils.hpp"

#include <glm/glm.hpp>


class Image {
  public:
    Image (glm::uvec2 size);

    void setData (const int *data);
    void resize (glm::uvec2 newSize);

    unsigned int getID () const { return texture; }
    glm::uvec2   getSize () const { return size; }

  private:
    unsigned int texture;
    glm::uvec2   size;
};
