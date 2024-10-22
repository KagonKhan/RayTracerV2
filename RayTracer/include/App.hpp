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
    void prepareBackground ();
    void renderApp ();
    void renderScene ();
    void renderObjectSettings ();
    void renderSettingsGUI ();

    MainWindow window{{800, 600}, "Raytracing"};
    Renderer   renderer;
    Scene      scene;
    Camera     camera{45.f, 0.1f, 1000.0f};
    float      dtime{0.f};
    float      render_time{0.f};


    struct ObjectEditor {
        ObjectEditor (Scene &s) : scene (s) {}

        void render ();

      private:
        void renderListOfObjects ();
        void renderObjectDetails ();

        Scene &scene;
        int    selectedIndex = -1;
    } editor{scene};
};
