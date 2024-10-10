#include "MainWindow.hpp"

#include <cstdio>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

MainWindow::MainWindow(Vec2f size, const std::string &name) {
  glfwSetErrorCallback(glfw_error_callback);

  if (!glfwInit())
    throw "Could not initialize glfw!";

  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window = glfwCreateWindow(size.x, size.y, name.c_str(), nullptr, nullptr);
  if (window == nullptr)
    throw("Could not create the window");

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); //

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad;              // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport
                                                      // / Platform Windows
  // io.ConfigViewportsNoAutoMerge = true;
  // io.ConfigViewportsNoTaskBarIcon = true;

  ImGui::StyleColorsDark();

  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

MainWindow::~MainWindow() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

void MainWindow::startFrame() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void MainWindow::render() {
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(0, 0, 0, 255);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  ImGuiIO &io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }

  glfwSwapBuffers(window);
}
