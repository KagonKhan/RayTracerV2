#pragma once
#include "MainWindow.hpp"

#include <glm/glm.hpp>

class Input {
  public:
    enum class MouseButton : short int {
        LEFT   = GLFW_MOUSE_BUTTON_LEFT,
        RIGHT  = GLFW_MOUSE_BUTTON_RIGHT,
        MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
    };
    enum class KeyButton : short int {
        W = GLFW_KEY_W,
        A = GLFW_KEY_A,
        S = GLFW_KEY_S,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        Q = GLFW_KEY_Q,
    };

    static glm::vec2 MousePosition () {
        double xpos, ypos;
        glfwGetCursorPos (*_window, &xpos, &ypos);
        return {(float) xpos, (float) ypos};
    }

    static bool IsButtonPressed (MouseButton button) {
        int state = glfwGetMouseButton (*_window, (int) button);
        return state == GLFW_PRESS;
    }

    static bool IsButtonPressed (KeyButton button) {
        int state = glfwGetKey (*_window, (int) button);
        return state == GLFW_PRESS;
    }

    static void Bind (MainWindow *window) { _window = window; }
    static void Unbind (MainWindow *window) { _window = nullptr; }


  private:
    inline static MainWindow *_window{nullptr};
};
