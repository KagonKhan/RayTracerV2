#pragma once
#include "Utils.hpp"

#include <glm/glm.hpp>


class Image {
  public:
    Image (glm::vec2 size);

    void setData (const int *data);
    void resize (glm::vec2 newSize);

    unsigned int getID () const { return texture; }
    glm::vec2    getSize () const { return size; }

  private:
    unsigned int texture;
    glm::vec2    size;
};
