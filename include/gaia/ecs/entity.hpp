#pragma once
#include <cstdint>

namespace gaia {
struct Entity {
    uint32_t index{UINT32_MAX};
    uint32_t generation{};
    [[nodiscard]] constexpr bool operator==(const Entity&) const = default;
    [[nodiscard]] constexpr bool valid() const { return index != UINT32_MAX; }
};
}  // namespace gaia
