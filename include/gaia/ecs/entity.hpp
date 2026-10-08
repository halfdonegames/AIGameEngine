#pragma once

#include <cstdint>
#include <compare>
#include <limits>

namespace gaia {

struct Entity {
    uint32_t Index = std::numeric_limits<uint32_t>::max();
    uint32_t Generation = 0;

    [[nodiscard]] bool IsValid() const { return Index != std::numeric_limits<uint32_t>::max(); }
    auto operator<=>(const Entity&) const = default;
};

} // namespace gaia
