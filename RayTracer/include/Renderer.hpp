#pragma once
#include "Camera.hpp"
#include "Image.hpp"
#include "RayObjects/Ray.hpp"
#include "RayObjects/Scene.hpp"
#include "Utils.hpp"

#include <vector>


namespace RayMath {
struct Color;
}
class Renderer {
  public:
    struct Settings {
        bool Accumulate = true;
        int  bounces    = 10;
    } settings;

  public:
    void                   onResize (glm::uvec2 newSize);
    void                   render (Camera const &camera, Scene const &scene);
    std::shared_ptr<Image> getImage () { return image; }
    void                   ResetFrameIndex () { m_FrameIndex = 1; }

  private:
    struct HitPayload {
        float     HitDistance;
        glm::vec3 WorldPosition;
        glm::vec3 WorldNormal;

        int ObjectIndex;
    };

    glm::vec4  perPixel (int x, int y);
    void       refraction (Renderer::HitPayload &payload, const Sphere &sphere, Ray &ray, const Material &material,
                           uint32_t &seed, glm::vec3 &contribution);
    HitPayload TraceRay (const Ray &ray);
    HitPayload ClosestHit (const Ray &ray, float hitDistance, int objectIndex);
    HitPayload Miss (const Ray &ray);


    std::shared_ptr<Image> image{new Image ({0, 0})};
    int                   *imageData          = nullptr;
    glm::vec4             *m_AccumulationData = nullptr;
    int                    m_FrameIndex{1};

    std::vector<int> m_ImageHorizontalIter, m_ImageVerticalIter;

    const Camera *camera;
    const Scene  *scene;
};
