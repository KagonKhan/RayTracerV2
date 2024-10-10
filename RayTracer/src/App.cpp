#include "App.hpp"
#include <ImGui/imgui_impl_glfw.h>

void App::start() {
  while (!glfwWindowShouldClose(window)) {

    glfwPollEvents();
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    window.startFrame();

    window.render();
  }
}
