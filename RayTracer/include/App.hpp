#pragma once

#include "Camera.hpp"
#include "ImGui/imgui.h"
#include "Image.hpp"
#include "MainWindow.hpp"
#include "Renderer.hpp"

#include <memory>

class App {
  public:
    App () = default;
    void start ();

  private:
    void renderApp ();
    void prepareBackground ();

    MainWindow window{{1920, 1080}, "Raytracing"};
    Renderer   renderer;
    Camera     camera{45.f, 0.1f, 1000.0f};
    float      dtime{0.f};
    float      render_time{0.f};
};
