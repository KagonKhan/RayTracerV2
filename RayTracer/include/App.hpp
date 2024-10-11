#pragma once

#include "ImGui/imgui.h"
#include "Image.hpp"
#include "MainWindow.hpp"
#include <memory>

class Renderer {
public:
  Renderer() {
    // int *image = (int *)malloc(1920 * 1080 * sizeof(int));

    // for (int y = 0; y < 1080; ++y)
    //   for (int x = 0; x < 1920; ++x) {
    //     image[y * 1920 + x] = 0xffff00ff;
    //   }

    // this->image->setData(image);
  }
  void onResize(Vec2i newSize) {
    if (newSize != image->getSize()) {
      image->resize(newSize);

      delete imageData;
      imageData = new int[newSize.x * newSize.y];
    }
  }
  std::shared_ptr<Image> render();

  std::shared_ptr<Image> image{new Image({0, 0})};
  int *imageData = nullptr;
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
