#pragma once

#include "RayMath/Ray.hpp"

#include <cmath>
#include <vector>


struct HitPayload {
    RayMath::Point  p;
    RayMath::Vector normal;
    float           t;
    bool            front_face;

    void setFaceNormal (RayMath::Ray const &r, RayMath::Vector const &outwardNormal) {
        front_face = r.direction.dot (outwardNormal) < 0;
        normal     = front_face ? outwardNormal : -outwardNormal;
    }
};

struct HitObject {
    virtual bool hit (RayMath::Ray const &ray, float ray_tmin, float ray_tmax) const = 0;
};

struct Sphere : public HitObject {
    float          radius;
    RayMath::Point center;

    Sphere (RayMath::Point center, float radius) : center (center), radius (radius) {}

    bool hit (RayMath::Ray const &ray, float ray_tmin, float ray_tmax) const override {
        auto  origin = RayMath::Vector (ray.origin.x, ray.origin.y, ray.origin.z);
        float a      = ray.direction.dot (ray.direction);
        float b      = 2.f * origin.dot (ray.direction);
        float c      = origin.dot (origin) - radius * radius;
        float d      = b * b - 4.f * a * c;

        if (d < 0.f)
            return false;


        float root = (-b - std::sqrtf (d)) / (2.f * a);


        if (root <= ray_tmin || ray_tmax <= root) {
            root = (-b + std::sqrtf (d)) / 2.f * a;
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }


        return true;
    }
};


struct World : public HitObject {
    std::vector<std::shared_ptr<HitObject>> objects;

    void clear () { objects.clear (); };
    void add (std::shared_ptr<HitObject> object) { objects.push_back (object); }

    bool hit (RayMath::Ray const &ray, float ray_tmin, float ray_tmax) const override {
        bool hit_anything   = false;
        auto closest_so_far = ray_tmax;

        for (const auto &object : objects) {
            if (object->hit (ray, ray_tmin, closest_so_far)) {
                hit_anything = true;
            }
        }

        return hit_anything;
    }
};
