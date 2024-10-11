#pragma once
#include <string>

#include <GLFW/glfw3.h>

struct Vec2i {
  int x;
  int y;
};
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
