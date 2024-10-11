#pragma once

#include "ImGui/imgui.h"
#include "MainWindow.hpp"

class Image {
public:
  Image(Vec2i size) : size(size) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound
    // texture object)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void update(const int *data) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, texture);
  }

  void draw() {

    ImGui::Image((ImTextureID)(intptr_t)texture,
                 ImGui::GetContentRegionAvail());
  }

private:
  unsigned int texture;
  Vec2i size;
};

class Renderer {
public:
  Renderer() {
    int *image = (int *)malloc(1920 * 1080 * sizeof(int));

    for (int y = 0; y < 1080; ++y)
      for (int x = 0; x < 1920; ++x) {
        image[y * 1920 + x] = 0xffff00ff;
      }

    this->image.update(image);
  }
  void render();

  Image image{{1920, 1080}};
};

class App {
public:
  void start();

private:
  void renderApp();
  void prepareBackground();
  MainWindow window{{1920, 1080}, "Raytracing"};
  Renderer renderer;
  float dtime{0.f};
};
