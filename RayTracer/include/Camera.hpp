#pragma once
#include "Utils.hpp"

#include <glm/glm.hpp>
#include <vector>


struct Camera {
    Camera (float verticalFOV, float nearClip, float farClip);

    bool onUpdate (float ts);
    void onResize (glm::vec2 newSize);

    const glm::vec3              &GetPosition () const { return m_Position; }
    const std::vector<glm::vec3> &GetRayDirections () const { return m_RayDirections; }

  private:
    void RecalculateView ();
    void RecalculateRayDirections ();
    void RecalculateProjection ();

  private:
    glm::mat4 m_View{1.f};
    glm::mat4 m_InverseView{1.f};
    glm::mat4 m_Projection{1.f};
    glm::mat4 m_InverseProjection{1.f};

    float m_VerticalFOV = 90.0f;
    float m_NearClip    = 0.1f;
    float m_FarClip     = 1000.0f;
    float rotationSpeed = 7.3f;

    glm::vec3 m_Position{0.0f, 0.0f, 9.0f};
    glm::vec3 m_ForwardDirection{0.0f, 0.0f, -1.f};

    // Cached ray directions
    std::vector<glm::vec3> m_RayDirections;

    glm::vec2  m_LastMousePosition{0.0f, 0.0f};
    glm::uvec2 m_ViewportSize;
};