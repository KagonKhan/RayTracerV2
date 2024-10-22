#pragma once
#include <compare>
#include <glm/glm.hpp>
#include <random>


class Random {
  public:
    static void     Init () { s_RandomEngine.seed (std::random_device () ()); }
    static uint32_t UInt () { return s_Distribution (s_RandomEngine); }
    static uint32_t UInt (uint32_t min, uint32_t max) {
        return min + (s_Distribution (s_RandomEngine) % (max - min + 1));
    }

    static float Float () {
        return (float) s_Distribution (s_RandomEngine) / (float) std::numeric_limits<uint32_t>::max ();
    }


    static glm::vec3 Vec3 () { return glm::vec3 (Float (), Float (), Float ()); }
    static glm::vec3 Vec3 (float min, float max) {
        return glm::vec3 (Float () * (max - min) + min, Float () * (max - min) + min, Float () * (max - min) + min);
    }
    static glm::vec3 InUnitSphere (uint32_t &seed) {
        return glm::normalize (glm::vec3 (RandomFloatPCG (seed) * 2.0f - 1.0f, RandomFloatPCG (seed) * 2.0f - 1.0f,
                                          RandomFloatPCG (seed) * 2.0f - 1.0f));
    }
    static glm::vec3 InUnitHemiSphere (uint32_t &seed, glm::vec3 const &normal) {
        glm::vec3 on_unit_sphere = InUnitSphere (seed);
        if (glm::dot (on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
            return on_unit_sphere;
        else
            return -on_unit_sphere;
    }


    static uint32_t PCG_Hash (uint32_t input) {
        uint32_t state = input * 747796405u + 2891336453u;
        uint32_t word  = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
        return (word >> 22u) ^ word;
    }

    static float RandomFloatPCG (uint32_t &seed) {
        seed = PCG_Hash (seed);
        return (float) seed / (float) std::numeric_limits<uint32_t>::max ();
    }

  private:
    inline static thread_local std::mt19937                                             s_RandomEngine;
    inline static thread_local std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};


constexpr uint32_t toRGBA (glm::vec4 const &color) noexcept {
    auto r = (uint8_t) (color.r * 255.0f);
    auto g = (uint8_t) (color.g * 255.0f);
    auto b = (uint8_t) (color.b * 255.0f);
    auto a = (uint8_t) (color.a * 255.0f);
    return ((uint8_t) a << 24) | ((uint8_t) b << 16) | ((uint8_t) g << 8) | ((uint8_t) r);
}

namespace Colors {
inline static glm::vec4 Black{0.0f, 0.0f, 0.0f, 1.0f};
inline static glm::vec4 White{1.0f, 1.0f, 1.0f, 1.0f};
inline static glm::vec4 Red{1.0f, 0.0f, 0.0f, 1.0f};
inline static glm::vec4 Lime{0.0f, 1.0f, 0.0f, 1.0f};
inline static glm::vec4 Blue{0.0f, 0.0f, 1.0f, 1.0f};
inline static glm::vec4 Yellow{1.0f, 1.0f, 0.0f, 1.0f};
inline static glm::vec4 Cyan{0.0f, 1.0f, 1.0f, 1.0f};
inline static glm::vec4 Magenta{1.0f, 0.0f, 1.0f, 1.0f};
inline static glm::vec4 Silver{0.75f, 0.75f, 0.75f, 1.0f};
inline static glm::vec4 Gray{0.5f, 0.5f, 0.5f, 1.0f};
inline static glm::vec4 Maroon{0.5f, 0.0f, 0.0f, 1.0f};
inline static glm::vec4 Olive{0.5f, 0.5f, 0.0f, 1.0f};
inline static glm::vec4 Green{0.0f, 0.5f, 0.0f, 1.0f};
inline static glm::vec4 Purple{0.5f, 0.0f, 0.5f, 1.0f};
inline static glm::vec4 Teal{0.0f, 0.5f, 0.5f, 1.0f};
inline static glm::vec4 Navy{0.0f, 0.0f, 0.5f, 1.0f};
} // namespace Colors