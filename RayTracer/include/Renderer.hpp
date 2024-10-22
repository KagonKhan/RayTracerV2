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
    void                   onResize (glm::uvec2 newSize);
    void                   render (Camera const &camera, Scene const &scene);
    std::shared_ptr<Image> getImage () { return image; }

  private:
    struct HitPayload {
        float     HitDistance;
        glm::vec3 WorldPosition;
        glm::vec3 WorldNormal;

        int ObjectIndex;
    };

    glm::vec4  perPixel (int x, int y);
    HitPayload TraceRay (const Ray &ray);
    HitPayload ClosestHit (const Ray &ray, float hitDistance, int objectIndex);
    HitPayload Miss (const Ray &ray);


    std::shared_ptr<Image> image{new Image ({0, 0})};
    int                   *imageData = nullptr;

    std::vector<int> m_ImageHorizontalIter, m_ImageVerticalIter;

    const Camera *camera;
    const Scene  *scene;
};
