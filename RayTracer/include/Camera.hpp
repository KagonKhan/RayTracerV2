#pragma once
#include "RayMath/Matrix.hpp"
#include "RayMath/Point.hpp"
#include "RayMath/Vector.hpp"
#include "Utils.hpp"


struct Camera {
    Camera (float verticalFOV, float nearClip, float farClip);

    bool onUpdate (float ts);
    void onResize (Vec2i size);

    const RayMath::Matrix              &GetProjection () const { return m_Projection; }
    const RayMath::Matrix              &GetInverseProjection () const { return m_InverseProjection; }
    const RayMath::Matrix              &GetView () const { return m_View; }
    const RayMath::Matrix              &GetInverseView () const { return m_InverseView; }
    const RayMath::Point               &GetPosition () const { return m_Position; }
    const RayMath::Vector              &GetDirection () const { return m_ForwardDirection; }
    const std::vector<RayMath::Vector> &GetRayDirections () const { return m_RayDirections; }

    float GetRotationSpeed ();

  private:
    void RecalculateProjection ();
    void RecalculateView ();
    void RecalculateRayDirections ();

  private:
    RayMath::Matrix m_Projection{1.f};
    RayMath::Matrix m_View{1.f};
    RayMath::Matrix m_InverseProjection{1.f};
    RayMath::Matrix m_InverseView{1.f};

    float m_VerticalFOV = 45.0f;
    float m_NearClip    = 0.1f;
    float m_FarClip     = 100.0f;

    RayMath::Point  m_Position{0.0f, 0.0f, 6.0f};
    RayMath::Vector m_ForwardDirection{0.0f, 0.0f, -1.f};

    // Cached ray directions
    std::vector<RayMath::Vector> m_RayDirections;

    Vec2f m_LastMousePosition{0.0f, 0.0f};

    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
};