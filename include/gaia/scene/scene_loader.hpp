#pragma once

#include "gaia/core/status.hpp"
#include "gaia/ecs/world.hpp"

#include <string>

namespace gaia {

class SceneLoader {
public:
    static Status LoadFromFile(const std::string& path, World& world);
    static Status LoadFromText(const std::string& text, World& world);
};

} // namespace gaia
