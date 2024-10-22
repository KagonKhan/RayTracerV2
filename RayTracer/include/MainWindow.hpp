#pragma once
#include "Utils.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

class MainWindow {
  public:
    MainWindow (glm::vec2 size, const std::string &name);
    ~MainWindow ();

    operator GLFWwindow * () const { return window; }

    void startFrame ();
    void render ();

  private:
    GLFWwindow *window{nullptr};
};
