#include "Camera.hpp"
#include "Input.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/mat4x4.hpp>


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

    return true;
}
void Camera::onResize (Vec2i size) {
    if (size.x == m_ViewportWidth && size.y == m_ViewportHeight)
        return;

    m_ViewportWidth  = size.x;
    m_ViewportHeight = size.y;

    RecalculateProjection ();
    RecalculateRayDirections ();


    float half_view = std::tanf (m_VerticalFOV / 2.f);
    float aspect    = m_ViewportWidth / m_ViewportHeight;
    if (aspect >= 1) {
        halfWidth  = half_view;
        halfHeight = half_view / aspect;
    } else {
        halfWidth  = half_view * aspect;
        halfHeight = half_view;
    }
    pxsize = 2 * halfWidth / m_ViewportWidth;
}

float Camera::GetRotationSpeed () { return 0.3f; }

float toRad (float deg) { return deg * 3.14159f / 180.f; }

void Camera::RecalculateProjection () {
    RayMath::Matrix result (.0f);

    // TODO: tan instead of 2 cos / sin calls?
    float rad = toRad (m_VerticalFOV);
    float h   = std::cosf (rad / 2.0f) / std::sinf (rad / 2.0f);
    float w   = h * (float) m_ViewportHeight / (float) m_ViewportWidth;

    // Create the perspective projection matrix
    result.data[RayMath::mapIndex (0, 0)] = w;
    result.data[RayMath::mapIndex (1, 1)] = h;
    result.data[RayMath::mapIndex (2, 2)] = -(m_FarClip + m_NearClip) / (m_FarClip - m_NearClip);
    result.data[RayMath::mapIndex (2, 3)] = -1.0f;
    result.data[RayMath::mapIndex (3, 2)] = -(2.0f * m_FarClip * m_NearClip) / (m_FarClip - m_NearClip);

    m_Projection        = result;
    m_InverseProjection = m_Projection.inversed ();

    std::cout << "\n\nMIN: \n";
    m_Projection.print ();
    std::cout << "\n\nGLM: \n"
              << glm::to_string (glm::perspectiveFov (glm::radians (m_VerticalFOV), (float) m_ViewportWidth, (float) m_ViewportHeight, m_NearClip, m_FarClip));
    void;
}

void Camera::RecalculateView () {
    RayMath::Vector up{0.f, 1.f, 0.f};

    m_View        = RayMath::Matrix::viewTransform (m_Position, RayMath::Point (0, 0, 0), up);
    m_InverseView = m_View.inversed ();
    m_Origin      = m_InverseView * RayMath::Point (0, 0, 0);
}

void Camera::RecalculateRayDirections () {
    m_RayDirections.resize (m_ViewportWidth * m_ViewportHeight);

    for (uint32_t y = 0; y < m_ViewportHeight; y++) {
        for (uint32_t x = 0; x < m_ViewportWidth; x++) {
            Vec2f coord = {(float) x, (float) y};


            // float xoffset = (coord.x + 0.5f) * pxsize;
            // float yoffset = (coord.y + 0.5f) * pxsize;
            // float world_x = halfWidth - xoffset;
            // float world_y = halfHeight - yoffset;
            // auto  pixel   = m_InverseView * RayMath::Point (world_x, world_y, -1);


            coord.x = (coord.x / (float) m_ViewportWidth) * 2.0f - 1.f;
            coord.y = (coord.y / (float) m_ViewportHeight) * 2.0f - 1.f;
            RayMath::Vector temp{coord.x, coord.y, 1, 1};
            auto            target = m_InverseProjection * temp;
            target                 = target / target.w;
            target.w               = 0;
            target                 = target.normalized ();

            auto dir = m_InverseView * target;
            dir.w    = 0;

            m_RayDirections[x + y * m_ViewportWidth] = dir;
            // m_RayDirections[x + y * m_ViewportWidth] = (pixel - GetOrigin ()).normalized ();
        }
    }
}
