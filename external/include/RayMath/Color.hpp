#pragma once
#include <algorithm>
#include <iostream>

namespace RayMath {
struct Color {
    float r{0.f}, g{0.f}, b{0.f}, a{255.f};

    inline constexpr Color () noexcept = default;
    inline constexpr Color (float r = 0.f, float g = 0.f, float b = 0.f, float a = 255.f) noexcept : r (r), g (g), b (b), a (a) {}

    inline constexpr Color operator+ (Color const &rhs) const noexcept { return {r + rhs.r, g + rhs.g, b + rhs.b}; }
    inline constexpr Color operator- (Color const &rhs) const noexcept { return {r - rhs.r, g - rhs.g, b - rhs.b}; }

    inline constexpr Color operator* (float scalar) const noexcept { return {r * scalar, g * scalar, b * scalar}; }
    inline constexpr Color operator* (Color const &rhs) const noexcept { return {r * rhs.r, g * rhs.g, b * rhs.b}; }
    // TODO: clamping
    inline constexpr Color clamped () const noexcept { return {std::clamp (r, 0.f, 255.f), std::clamp (g, 0.f, 255.f), std::clamp (b, 0.f, 255.f)}; }

    constexpr auto operator<=> (Color const &) const noexcept = default;

    constexpr uint32_t toRGBA () noexcept { return ((uint8_t) a << 24) | ((uint8_t) b << 16) | ((uint8_t) g << 8) | ((uint8_t) r); }
};

namespace Colors {
inline static Color Black{0.f, 0.f, 0.f};
inline static Color White{255.f, 255.f, 255.f};
inline static Color Red{255.f, 0.f, 0.f};
inline static Color Lime{0.f, 255.f, 0.f};
inline static Color Blue{0.f, 0.f, 255.f};
inline static Color Yellow{255.f, 255.f, 0.f};
inline static Color Cyan{0.f, 255.f, 255.f};
inline static Color Magenta{255.f, 0.f, 255.f};
inline static Color Silver{192, 192, 192};
inline static Color Gray{128.f, 128.f, 128.f};
inline static Color Maroon{128.f, 0.f, 0.f};
inline static Color Olive{128.f, 128.f, 0.f};
inline static Color Green{0.f, 128.f, 0.f};
inline static Color Purple{128.f, 0.f, 128.f};
inline static Color Teal{0.f, 128.f, 128.f};
inline static Color Navy{0.f, 0.f, 128.f};
} // namespace Colors
} // namespace RayMath