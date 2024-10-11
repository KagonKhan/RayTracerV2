#include "App.hpp"
#include <ImGui/imgui_impl_glfw.h>
#include <chrono>

void App::start() {
  Random::Init();

  auto lastTime = std::chrono::high_resolution_clock::now();
  while (!glfwWindowShouldClose(window)) {
    auto now = std::chrono::high_resolution_clock::now();
    dtime =
        std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime)
            .count();
    lastTime = now;
    auto lastTime = std::chrono::high_resolution_clock::now();
    glfwPollEvents();

    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    window.startFrame();
    prepareBackground();
    renderApp();
    window.render();
  }
}

void App::prepareBackground() {
  const static ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace Demo", nullptr, window_flags);
  ImGui::PopStyleVar(3);

  // Submit the DockSpace
  ImGuiIO &io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGui::DockSpace(ImGui::GetID("AppDockspace"), ImVec2(0.0f, 0.0f),
                     ImGuiDockNodeFlags_NoUndocking);
  }
  ImGui::End();
}

void App::renderApp() {
  ImGui::Begin("Settings");
  ImGui::Text("%.3f ms", dtime / 1000.f);
  ImGui::End();

  ImGui::Begin("Scene");

  auto width = ImGui::GetContentRegionAvail().x;
  auto height = ImGui::GetContentRegionAvail().y;
  renderer.onResize({(int)width, (int)height});
  auto image = renderer.render();

  ImGui::Image((ImTextureID)(intptr_t)image->getID(),
               {(float)image->getSize().x, (float)image->getSize().y},
               ImVec2(0, 1), ImVec2(1, 0));

  ImGui::End();
}
