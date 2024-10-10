#pragma once
#include <string>

#include <GLFW/glfw3.h>

struct Vec2f {
  float x;
  float y;
};
class MainWindow {
public:
  MainWindow(Vec2f size, const std::string &name);
  ~MainWindow();

  operator GLFWwindow *() const { return window; }

  void startFrame();
  void render();

private:
  GLFWwindow *window{nullptr};
};
