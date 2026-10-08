#pragma once
#include <string>
#include "gaia/core/status.hpp"
#include "gaia/ecs/world.hpp"
namespace gaia {
class SceneLoader {
  public:
    static Status LoadJson(const std::string& source, World& destination);
};
}  // namespace gaia
