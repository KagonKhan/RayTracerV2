#include "Camera.hpp"
#include "ImGui/imgui.h"
#include "Input.hpp"
Camera::Camera (float verticalFOV, float nearClip, float farClip) : m_VerticalFOV (verticalFOV), m_NearClip (nearClip), m_FarClip (farClip) {}

bool Camera::onUpdate (float ts) {
    Vec2f mousePos = Input::MousePosition ();
    Vec2f moveDelta{
        .x = (mousePos.x - m_LastMousePosition.x) * 0.002f,
        .y = (mousePos.y - m_LastMousePosition.y) * 0.002f,
    };
    m_LastMousePosition = mousePos;


    // Movement
    float           speed = 5.0f;
    bool            moved = false;
    RayMath::Vector upDirection (0.0f, 1.0f, 0.0f);
    RayMath::Vector rightDirection = m_ForwardDirection.cross (upDirection);

    if (Input::IsButtonPressed (Input::KeyButton::W)) {
        m_Position = m_Position + m_ForwardDirection * speed * ts;
        moved      = true;
    } else if (Input::IsButtonPressed (Input::KeyButton::S)) {
        m_Position = m_Position - m_ForwardDirection * speed * ts;
        moved      = true;
    }
    if (Input::IsButtonPressed (Input::KeyButton::A)) {
        m_Position = m_Position - rightDirection * speed * ts;
        moved      = true;
    } else if (Input::IsButtonPressed (Input::KeyButton::D)) {
        m_Position = m_Position + rightDirection * speed * ts;
        moved      = true;
    }
    if (Input::IsButtonPressed (Input::KeyButton::Q)) {
        m_Position = m_Position - upDirection * speed * ts;
        moved      = true;
    } else if (Input::IsButtonPressed (Input::KeyButton::E)) {
        m_Position = m_Position + upDirection * speed * ts;
        moved      = true;
    }


    if (moved) {
        RecalculateView ();
        RecalculateRayDirections ();
    }
}

void Camera::onResize (Vec2i size) {
    if (size.x == m_ViewportWidth && size.y == m_ViewportHeight)
        return;

    m_ViewportWidth  = size.x;
    m_ViewportHeight = size.y;

    RecalculateProjection ();
    RecalculateRayDirections ();
}

float Camera::GetRotationSpeed () { return 0.3f; }

void Camera::RecalculateProjection () {
    float aspect = m_ViewportWidth / m_ViewportHeight;

    // Calculate the vertical field of view (fovY)
    float fovY = 2.0f * atan (tan (m_VerticalFOV / 2.0f) / aspect); // TODO: toRadians?

    // Initialize the perspective matrix to identity
    RayMath::Matrix result (1.0f);

    // Calculate the tangent of half of the vertical field of view
    float tanHalfFovy = tan (fovY / 2.0f);

    // Create the perspective projection matrix
    result.data[0]         = 1.0f / (aspect * tanHalfFovy);
    result.data[5]         = 1.0f / tanHalfFovy;
    result.data[2 * 4 + 2] = -(m_FarClip + m_NearClip) / (m_FarClip - m_NearClip);
    result.data[2 * 4 + 3] = -1.0f;
    result.data[3 * 4 + 2] = -(2.0f * m_FarClip * m_NearClip) / (m_FarClip - m_NearClip);
    result.data[3 * 4 + 3] = 0.0f;
    m_Projection           = result;
    m_InverseProjection    = m_Projection.inversed ();
}


void Camera::RecalculateView () {
    RayMath::Vector up{0.f, 1.f, 0.f};
    RayMath::Vector forward = (m_Position + m_ForwardDirection - m_Position).normalized ();
    RayMath::Vector left    = forward.cross (up.normalized ());
    RayMath::Vector trueUp  = left.cross (forward);
    RayMath::Vector nul{0.f, 0.f, 0.f, 1.f};

    RayMath::Matrix orientation{left, trueUp, -forward, nul};

    m_View        = orientation.translated (-m_Position.x, -m_Position.y, -m_Position.z);
    m_InverseView = m_View.inversed ();
}

void Camera::RecalculateRayDirections () {
    m_RayDirections.resize (m_ViewportWidth * m_ViewportHeight);

    for (uint32_t y = 0; y < m_ViewportHeight; y++) {
        for (uint32_t x = 0; x < m_ViewportWidth; x++) {
            Vec2f coord = {(float) x / (float) m_ViewportWidth, (float) y / (float) m_ViewportHeight};
            coord.x     = coord.x * 2.0f - 1.0f; // -1 -> 1
            coord.y     = coord.y * 2.0f - 1.0f; // -1 -> 1

            RayMath::Point target       = m_InverseProjection * RayMath::Point (coord.x, coord.y, 1, 1);
            auto           targetVector = RayMath::Vector (target.x, target.y, target.z, target.w);
            auto           normal       = (targetVector / targetVector.w).normalized ();
            normal.w                    = 0;
            auto rayDirection           = (m_InverseView * normal); // World space;


            m_RayDirections[x + y * m_ViewportWidth] = rayDirection;
        }
    }
}
