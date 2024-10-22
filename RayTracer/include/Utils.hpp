#pragma once
#include <compare>
#include <glm/glm.hpp>
#include <random>


class Random {
  public:
    static void     Init () { s_RandomEngine.seed (std::random_device () ()); }
    static uint32_t UInt () { return s_Distribution (s_RandomEngine); }
    static uint32_t UInt (uint32_t min, uint32_t max) { return min + (s_Distribution (s_RandomEngine) % (max - min + 1)); }

    static float Float () { return (float) s_Distribution (s_RandomEngine) / (float) std::numeric_limits<uint32_t>::max (); }

  private:
    inline static std::mt19937                                             s_RandomEngine;
    inline static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};


constexpr uint32_t toRGBA (glm::vec4 color) noexcept {
    return ((uint8_t) color.a << 24) | ((uint8_t) color.b << 16) | ((uint8_t) color.g << 8) | ((uint8_t) color.r);
}

namespace Colors {
inline static glm::vec4 Black{ 0.f, 0.f, 0.f, 255.f };
inline static glm::vec4 White{ 255.f, 255.f, 255.f, 255.f };
inline static glm::vec4 Red{ 255.f, 0.f, 0.f, 255.f };
inline static glm::vec4 Lime{ 0.f, 255.f, 0.f, 255.f };
inline static glm::vec4 Blue{ 0.f, 0.f, 255.f, 255.f };
inline static glm::vec4 Yellow{ 255.f, 255.f, 0.f, 255.f };
inline static glm::vec4 Cyan{ 0.f, 255.f, 255.f, 255.f };
inline static glm::vec4 Magenta{ 255.f, 0.f, 255.f, 255.f };
inline static glm::vec4 Silver{ 192, 192, 192, 255.f };
inline static glm::vec4 Gray{ 128.f, 128.f, 128.f, 255.f };
inline static glm::vec4 Maroon{ 128.f, 0.f, 0.f, 255.f };
inline static glm::vec4 Olive{ 128.f, 128.f, 0.f, 255.f };
inline static glm::vec4 Green{ 0.f, 128.f, 0.f, 255.f };
inline static glm::vec4 Purple{ 128.f, 0.f, 128.f, 255.f };
inline static glm::vec4 Teal{ 0.f, 128.f, 128.f, 255.f };
inline static glm::vec4 Navy{ 0.f, 0.f, 128.f, 255.f };
} // namespace Colors