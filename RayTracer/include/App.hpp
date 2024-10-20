#pragma once

#include "Camera.hpp"
#include "ImGui/imgui.h"
#include "Image.hpp"
#include "MainWindow.hpp"
#include "Renderer.hpp"

#include <memory>

class App {
  public:
    void start ();

  private:
    void renderApp ();
    void prepareBackground ();

    MainWindow window{{1920, 1080}, "Raytracing"};
    Renderer   renderer;
    Camera     camera{45.0f, 0.1f, 100.0f};
    float      dtime{0.f};
    float      render_time{0.f};
};
