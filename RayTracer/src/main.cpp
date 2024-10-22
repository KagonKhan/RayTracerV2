#include "App.hpp"

#include <iostream>


int main () {
    App app;
    app.start ();

    return 0;
}


/* RENDERING FLOW
#pragma once

#include "RayMath/Color.hpp"
#include "RayMath/Matrix.hpp"
#include "RayMath/Ray.hpp"

#include <cmath>
#include <ranges>
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


struct Material {
    float          ambient{};
    float          diffuse{};
    float          specular{};
    float          shininess{};
    RayMath::Color color{};

    float reflective{};
    float transparency{};
    float refraction{};
};

struct Intersection;
struct HitObject {
    Material        material;
    RayMath::Matrix transform{1.f};
    RayMath::Matrix inverseTransform{1.f};

    void setTransform (RayMath::Matrix const &transform) {
        this->transform  = transform;
        inverseTransform = transform.inversed ();
    }

    RayMath::Vector normalAt (RayMath::Point const &point) const {
        auto local_point  = inverseTransform * point;
        auto local_normal = localNormalAt (local_point);
        auto world_normal = inverseTransform * local_normal; // TODO: transpose the inverse?
        world_normal.w    = 0;

        return world_normal.normalized ();
    }

    RayMath::Ray transformRay (RayMath::Ray const &ray, RayMath::Matrix transform) const { return {transform * ray.origin, transform * ray.direction}; }

    std::vector<Intersection> intersectionsWith (RayMath::Ray const &ray) { return localIntersectionsWith (transformRay (ray, inverseTransform)); }

  protected:
    virtual std::vector<Intersection> localIntersectionsWith (RayMath::Ray const &ray)  = 0;
    virtual RayMath::Vector           localNormalAt (RayMath::Point const &point) const = 0;
};

struct Computations {
    float           t;
    HitObject      *obj;
    RayMath::Point  point;
    RayMath::Point  over_point;
    RayMath::Point  under_point;
    RayMath::Vector eye;
    RayMath::Vector normal;
    RayMath::Vector reflect;
    bool            inside;

    double n1, n2;
    Computations (float t, HitObject *obj, RayMath::Point p, RayMath::Vector eye, RayMath::Vector norm, bool inside) {
        this->t          = t;
        this->obj        = obj;
        this->point      = p;
        this->eye        = eye;
        this->normal     = norm;
        this->inside     = inside;
        this->over_point = p;

        over_point  = p + normal * (0.0001f);
        under_point = p - normal * (0.0001f);
        reflect     = (-eye).reflect (normal);

        n1 = n2 = 0;
    }

    double Schlick () {
        double cos = eye.dot (normal);

        if (n1 > n2) {
            double n      = n1 / n2;
            double sin_2t = n * n * (1 - cos * cos);

            if (sin_2t > 1.0)
                return 1.0;

            double cos_t = std::sqrtf (1.0 - sin_2t);
            cos          = cos_t;
        }

        double r0 = std::powf ((n1 - n2) / (n1 + n2), 2);
        return r0 + (1 - r0) * std::powf (1 - cos, 5);
    }
};


struct Intersection {
    Intersection () = default;
    HitObject *obj;
    float      t;
    Intersection (HitObject *object, float t) : obj (object), t (t) {}

    constexpr auto operator<=> (Intersection const &) const = default;

    Computations Compute (RayMath::Ray r, std::vector<Intersection> xs) const {
        RayMath::Point  pos  = r.at (t);
        RayMath::Vector norm = obj->normalAt (pos);
        bool            inside;

        if (norm.dot (-r.direction) < 0) {
            inside = true;
            norm   = -norm;
        } else
            inside = false;

        Computations retVal (t, obj, pos, -r.direction.normalized (), norm.normalized (), inside);


        if (xs.empty ())
            return retVal;


        std::vector<HitObject *> containers{};

        for (int i = 0; i < xs.size (); i++) {
            if (*this == xs[i]) {
                if (containers.size () == 0)
                    retVal.n1 = 1.0;
                else
                    retVal.n1 = containers.back ()->material.refraction;
            }


            std::ranges::remove_if (containers, [left = xs[i].obj] (HitObject *object) { return object == left; });

            containers.push_back (xs[i].obj);

            if (*this == xs[i]) {
                if (containers.size () == 0)
                    retVal.n2 = 1.0;
                else
                    retVal.n2 = containers.back ()->material.refraction;
                break;
            }
        }

        return retVal;
    }
};

struct Sphere : public HitObject {
    float          radius;
    RayMath::Point center;


    std::vector<Intersection> localIntersectionsWith (RayMath::Ray const &ray) override {
        std::vector<Intersection> retVal;
        RayMath::Vector           sphereToRay = ray.origin - RayMath::Point (0, 0, 0);

        float a = ray.direction.dot (ray.direction);
        float b = 2 * ray.direction.dot (sphereToRay);
        float c = sphereToRay.dot (sphereToRay) - 1;

        float delta = b * b - 4 * a * c;

        if (delta < 0)
            return retVal;
        else {
            float t1 = (-b - std::sqrtf (delta)) / (2.f * a);
            float t2 = (-b + std::sqrtf (delta)) / (2.f * a);

            retVal.emplace_back (this, t1);
            retVal.emplace_back (this, t2);
            return retVal;
        }
    };
    RayMath::Vector localNormalAt (RayMath::Point const &point) const override {
        RayMath::Vector norm = point - RayMath::Point{};
        norm.w               = 0;
        return norm.normalized ();
    };
};


struct Light {
    RayMath::Point pos;
    RayMath::Color intensity;
    Light () = default;


    virtual RayMath::Color Lighting (Material m, HitObject *obj, RayMath::Point const &p, RayMath::Vector const &eye, RayMath::Vector const &norm,
                                     bool shadowed = false) const = 0;
};

struct PointLight : public Light {
    PointLight (RayMath::Point p, RayMath::Color i) {
        pos       = p;
        intensity = i;
    }

    RayMath::Color Lighting (Material m, HitObject *obj, RayMath::Point const &p, RayMath::Vector const &eye, RayMath::Vector const &norm,
                             bool shadowed = false) const override {
        RayMath::Color eff_color = m.color;
        eff_color *= intensity;

        RayMath::Color ambient = eff_color * m.ambient;


        RayMath::Vector lightV = (pos - p).normalized ();

        RayMath::Color dif;
        RayMath::Color spec;

        double light_dot_normal = lightV.dot (norm);
        if (light_dot_normal < 0 || shadowed) {
            dif  = RayMath::Colors::Black;
            spec = RayMath::Colors::Black;
        } else {
            dif = eff_color * m.diffuse * light_dot_normal;

            RayMath::Vector reflectv        = (-lightV).reflect (norm);
            double          reflect_dot_eye = reflectv.dot (eye);

            if (reflect_dot_eye <= 0)
                spec = RayMath::Colors::Black;
            else {
                double factor = std::powf (reflect_dot_eye, m.shininess);
                spec          = intensity * m.specular * factor;
            }
        }
        RayMath::Color c = ambient + dif + spec;
        return c;
    }
};


struct World {
    std::vector<std::shared_ptr<HitObject>> objects;
    std::vector<std::shared_ptr<Light>>     lights;

    void clear () {
        objects.clear ();
        lights.clear ();
    };
    void add (std::shared_ptr<HitObject> object) { objects.push_back (object); }
    void add (std::shared_ptr<Light> light) { lights.push_back (light); }

    std::vector<Intersection> hit (RayMath::Ray const &ray, float ray_tmin, float ray_tmax) const {
        std::vector<Intersection> retVal;
        for (const auto &object : objects) {
            retVal.append_range (object->intersectionsWith (ray));
        }
        std::ranges::sort (retVal, {}, &Intersection::t);
        return retVal;
    }


    RayMath::Color Shading (Computations comp, int remaining) {
        RayMath::Color retVal = RayMath::Colors::Black;

        int lightIndex = 0;
        for (auto light : lights) {
            bool shaded = IsShadowed (comp.over_point, lightIndex);
            retVal += light->Lighting (comp.obj->material, comp.obj, comp.point, comp.eye, comp.normal, shaded);

            lightIndex++;
        }

        RayMath::Color reflected = ReflectiveShading (comp, remaining);
        // RayMath::Color refracted = RefractiveShading (comp, remaining);

        Material mat = comp.obj->material;

        // if (mat.reflective > 0 && mat.transparency > 0) {
        //     double reflectance = comp.Schlick ();
        //     return retVal + (reflected * reflectance) + (refracted * (1 - reflectance));
        // }

        return retVal + reflected; // + refracted;
    }

    bool IsShadowed (RayMath::Point p, int lightIndex) {
        RayMath::Vector v = lights[lightIndex]->pos - p;

        double          distance  = v.magnitude ();
        RayMath::Vector direction = v.normalized ();

        RayMath::Ray ray (p, direction);

        std::vector<Intersection> xs = hit (ray, 0, 999999);
        Intersection              h  = getPositive (xs);

        if (h.obj != nullptr && h.t < distance)
            return true;
        else
            return false;
    }
    RayMath::Color Coloring (RayMath::Ray ray, int remaining) {
        auto xs  = hit (ray, 0, 99999999999);
        auto hit = getPositive (xs);

        if (hit.obj != nullptr) {
            auto comps = hit.Compute (ray, xs);
            return Shading (comps, remaining);
        }
    }

    RayMath::Color ReflectiveShading (Computations comp, int remaining) {
        if (comp.obj->material.reflective == 0 || remaining <= 0)
            return RayMath::Color (0, 0, 0);


        RayMath::Ray   reflect_ray (comp.over_point, comp.reflect);
        RayMath::Color color = Coloring (reflect_ray, remaining - 1);

        return color * comp.obj->material.reflective;
    }


    RayMath::Color RefractiveShading (Computations comp, int remaining) {
        if (comp.obj->material.transparency == 0 || remaining <= 0)
            return RayMath::Color (0, 0, 0);


        double ratio  = comp.n1 / comp.n2;
        double cos_i  = comp.eye.dot (comp.normal);
        double sin_2t = ratio * ratio * (1.0 - cos_i * cos_i);

        if (sin_2t > 1)
            return RayMath::Color (0, 0, 0);

        double          cos_t = std::sqrtf (1.0 - sin_2t);
        RayMath::Vector dir   = comp.normal * (ratio * cos_i - cos_t) - comp.eye * ratio;

        RayMath::Ray ref_ray (comp.under_point, dir);

        return Coloring (ref_ray, remaining - 1) * comp.obj->material.transparency;
    }
    Intersection getPositive (std::vector<Intersection> &xs) {
        std::ranges::sort (xs, {}, &Intersection::t);
        for (auto x : xs | std::views::filter ([] (Intersection const &x) { return x.t > 0.f; })) {
            return x;
        }
        return {};
    }
};

*/