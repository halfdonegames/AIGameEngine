#include "gaia/scripting/control_api.hpp"

#include <sstream>

namespace gaia {
Result<Entity> ControlApi::Execute(const std::string& command) {
    std::istringstream input(command);
    std::string operation;
    input >> operation;
    if (operation == "spawn") {
        std::string name;
        std::getline(input >> std::ws, name);
        return m_World.CreateEntity(name);
    }
    uint32_t index = 0;
    uint32_t generation = 0;
    if (!(input >> index >> generation)) return Result<Entity>::Failure("Expected entity index and generation");
    const Entity entity {index, generation};
    if (operation == "destroy") {
        const auto status = m_World.DestroyEntity(entity);
        return status.IsOk() ? Result<Entity>::Success(entity) : Result<Entity>::Failure(status.Message());
    }
    if (operation == "remove") {
        std::string component;
        if (!(input >> component)) return Result<Entity>::Failure("Expected component name");
        const auto status = m_World.RemoveComponent(entity, component);
        return status.IsOk() ? Result<Entity>::Success(entity) : Result<Entity>::Failure(status.Message());
    }
    if (operation == "transform") {
        TransformComponent component;
        if (!(input >> component.Position[0] >> component.Position[1] >> component.Position[2] >> component.RotationDegrees[0] >> component.RotationDegrees[1] >> component.RotationDegrees[2] >> component.Scale[0] >> component.Scale[1] >> component.Scale[2])) return Result<Entity>::Failure("Expected nine transform values");
        const auto status = m_World.SetTransform(entity, component);
        return status.IsOk() ? Result<Entity>::Success(entity) : Result<Entity>::Failure(status.Message());
    }
    return Result<Entity>::Failure("Unknown control command: " + operation);
}
} // namespace gaia
