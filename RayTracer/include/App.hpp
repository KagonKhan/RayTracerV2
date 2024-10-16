#pragma once

#include "ImGui/imgui.h"
#include "Image.hpp"
#include "MainWindow.hpp"
#include "Renderer.hpp"
#include <memory>

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
