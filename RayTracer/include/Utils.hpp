#pragma once
#include <compare>

struct Vec2i {
  int x;
  int y;

  constexpr auto operator<=>(Vec2i const &) const noexcept = default;
};