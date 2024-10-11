#pragma once

#include "ImGui/imgui.h"
#include "MainWindow.hpp"

class App {
public:
  void start();

private:
  void renderApp();
  void prepareBackground();
  MainWindow window{{1920, 1080}, "Raytracing"};
};
