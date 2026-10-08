#pragma once

#include "gaia/core/status.hpp"
#include "gaia/ecs/world.hpp"

#include <string>

namespace gaia {

class ControlApi {
public:
    explicit ControlApi(World& world) : m_World(world) {}
    Result<Entity> Execute(const std::string& command);

private:
    World& m_World;
};

} // namespace gaia
