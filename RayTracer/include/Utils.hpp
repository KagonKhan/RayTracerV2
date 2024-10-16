#pragma once
#include <compare>
#include <random>

template <typename T> struct Vec2 {
  T x;
  T y;

  constexpr auto operator<=>(Vec2<T> const &) const noexcept = default;
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;

class Random {
public:
  static void Init() { s_RandomEngine.seed(std::random_device()()); }
  static uint32_t UInt() { return s_Distribution(s_RandomEngine); }
  static uint32_t UInt(uint32_t min, uint32_t max) {
    return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
  }

  static float Float() {
    return (float)s_Distribution(s_RandomEngine) /
           (float)std::numeric_limits<uint32_t>::max();
  }

private:
  inline static std::mt19937 s_RandomEngine;
  inline static std::uniform_int_distribution<std::mt19937::result_type>
      s_Distribution;
};