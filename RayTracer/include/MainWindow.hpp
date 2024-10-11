#pragma once
#include <string>

#include <GLFW/glfw3.h>

#include "Utils.hpp"
class MainWindow {
public:
  MainWindow(Vec2i size, const std::string &name);
  ~MainWindow();

  operator GLFWwindow *() const { return window; }

  void startFrame();
  void render();

private:
  GLFWwindow *window{nullptr};
};
