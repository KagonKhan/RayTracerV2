#include "Camera.hpp"
#include "Input.hpp"

#include <fmt/format.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/mat4x4.hpp>


Camera::Camera (float verticalFOV, float nearClip, float farClip) : m_VerticalFOV (verticalFOV), m_NearClip (nearClip), m_FarClip (farClip) {}

bool Camera::onUpdate (float ts) {
    glm::vec2 mousePos  = Input::MousePosition ();
    glm::vec2 moveDelta = (mousePos - m_LastMousePosition) * 0.0002f;
    m_LastMousePosition = mousePos;


    // Movement
    float     speed = 5.0f;
    bool      moved = false;
    glm::vec3 upDirection (0.0f, 1.0f, 0.0f);
    glm::vec3 rightDirection = glm::cross (m_ForwardDirection, upDirection);

    if (Input::IsButtonPressed (Input::KeyButton::W)) {
        m_Position += m_ForwardDirection * speed * ts;
        moved = true;
    } else if (Input::IsButtonPressed (Input::KeyButton::S)) {
        m_Position -= m_ForwardDirection * speed * ts;
        moved = true;
    }
    if (Input::IsButtonPressed (Input::KeyButton::A)) {
        m_Position -= rightDirection * speed * ts;
        moved = true;
    } else if (Input::IsButtonPressed (Input::KeyButton::D)) {
        m_Position += rightDirection * speed * ts;
        moved = true;
    }
    if (Input::IsButtonPressed (Input::KeyButton::Q)) {
        m_Position -= upDirection * speed * ts;
        moved = true;
    } else if (Input::IsButtonPressed (Input::KeyButton::E)) {
        m_Position += upDirection * speed * ts;
        moved = true;
    }

    if (Input::IsButtonPressed (Input::MouseButton::RIGHT)) {
        if (moveDelta.x != 0.0f || moveDelta.y != 0.0f) {
            float pitchDelta = moveDelta.y * rotationSpeed;
            float yawDelta   = moveDelta.x * rotationSpeed;

            glm::quat q = glm::normalize (glm::cross (glm::angleAxis (-pitchDelta, rightDirection), glm::angleAxis (-yawDelta, glm::vec3 (0.f, 1.0f, 0.0f))));
            m_ForwardDirection = glm::rotate (q, m_ForwardDirection);

            moved = true;
        }
    }


    if (moved) {
        RecalculateView ();
        RecalculateRayDirections ();
    }


    return moved;
}
void Camera::onResize (glm::vec2 newSize) {
    if (m_ViewportSize == (glm::uvec2) newSize)
        return;

    m_ViewportSize = (glm::uvec2) newSize;

   RecalculateProjection ();
   RecalculateRayDirections ();
}


void Camera::RecalculateProjection () {
    m_Projection        = glm::perspectiveFov (glm::radians (m_VerticalFOV), (float) m_ViewportSize.x, (float) m_ViewportSize.y, m_NearClip, m_FarClip);
    m_InverseProjection = glm::inverse (m_Projection);
}

void Camera::RecalculateView () {
    m_View        = glm::lookAt (m_Position, m_Position + m_ForwardDirection, glm::vec3 (0, 1, 0));
    m_InverseView = glm::inverse (m_View);
}

void Camera::RecalculateRayDirections () {
    m_RayDirections.resize (m_ViewportSize.x * m_ViewportSize.y);

    for (uint32_t y = 0; y < m_ViewportSize.y; y++) {
        for (uint32_t x = 0; x < m_ViewportSize.x; x++) {
            glm::vec2 coord = {(float) x / (float) m_ViewportSize.x, (float) y / (float) m_ViewportSize.y};
            coord           = coord * 2.0f - 1.0f; // -1 -> 1

            glm::vec4 target       = m_InverseProjection * glm::vec4 (coord.x, coord.y, 1, 1);
            glm::vec3 rayDirection = glm::vec3 (m_InverseView * glm::vec4 (glm::normalize (glm::vec3 (target) / target.w), 0)); // World space
            int       index        = x + y * m_ViewportSize.x;
            m_RayDirections[index] = rayDirection;
        }
    }
}
