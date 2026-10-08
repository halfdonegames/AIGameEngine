#include "gaia/ecs/world.hpp"

#include <cmath>
#include <limits>
#include <utility>

namespace gaia {
namespace {
bool IsFinitePositive(float value) { return std::isfinite(value) && value > 0.0F; }
}

Result<Entity> World::CreateEntity(std::string name) {
    if (name.empty()) return Result<Entity>::Failure("Entity name cannot be empty");
    uint32_t index = 0;
    if (m_FreeIndices.empty()) { index = static_cast<uint32_t>(m_Entities.size()); m_Entities.push_back({}); }
    else { index = m_FreeIndices.back(); m_FreeIndices.pop_back(); }
    auto& slot = m_Entities[index]; slot.Alive = true; ++m_AliveCount;
    return Result<Entity>::Success({index, slot.Generation});
}

Status World::ValidateAlive(Entity entity) const {
    if (!entity.IsValid() || entity.Index >= m_Entities.size()) return Status::Error("Invalid entity ID");
    const auto& slot = m_Entities[entity.Index];
    if (!slot.Alive || slot.Generation != entity.Generation) return Status::Error("Stale or destroyed entity ID");
    return Status::Ok();
}

bool World::IsAlive(Entity entity) const { return ValidateAlive(entity).IsOk(); }

Status World::DestroyEntity(Entity entity) {
    const auto status = ValidateAlive(entity); if (!status.IsOk()) return status;
    if (m_Entities[entity.Index].Generation == std::numeric_limits<uint32_t>::max()) return Status::Error("Entity generation exhausted");
    m_Transforms.Remove(entity); m_Renderables.Remove(entity); m_RigidBodies.Remove(entity); m_AudioEmitters.Remove(entity); m_UtilityAgents.Remove(entity); m_Scripts.Remove(entity);
    auto& slot = m_Entities[entity.Index]; slot.Alive = false;
    ++slot.Generation; m_FreeIndices.push_back(entity.Index); --m_AliveCount; return Status::Ok();
}

Status World::SetTransform(Entity entity, TransformComponent component) {
    const auto status = ValidateAlive(entity); if (!status.IsOk()) return status;
    for (const auto value : component.Scale) if (!IsFinitePositive(value)) return Status::Error("Transform scale must be finite and positive");
    m_Transforms.InsertOrAssign(entity, std::move(component)); return Status::Ok();
}
Status World::SetRenderable(Entity entity, RenderableComponent component) { const auto status = ValidateAlive(entity); if (!status.IsOk()) return status; if (component.Mesh.empty() || component.Material.empty()) return Status::Error("Renderable requires mesh and material"); m_Renderables.InsertOrAssign(entity, std::move(component)); return Status::Ok(); }
Status World::SetRigidBody(Entity entity, RigidBodyComponent component) { const auto status = ValidateAlive(entity); if (!status.IsOk()) return status; if (!IsFinitePositive(component.Mass) || !IsFinitePositive(component.GravityScale) || !IsFinitePositive(component.AtmosphericDensity) || !IsFinitePositive(component.Viscosity)) return Status::Error("Rigid body parameters must be finite and positive"); m_RigidBodies.InsertOrAssign(entity, component); return Status::Ok(); }
Status World::SetAudioEmitter(Entity entity, AudioEmitterComponent component) { const auto status = ValidateAlive(entity); if (!status.IsOk()) return status; if (component.Clip.empty() || !std::isfinite(component.Gain) || component.Gain < 0.0F) return Status::Error("Audio emitter has invalid clip or gain"); m_AudioEmitters.InsertOrAssign(entity, std::move(component)); return Status::Ok(); }
Status World::SetUtilityAgent(Entity entity, UtilityAgentComponent component) { const auto status = ValidateAlive(entity); if (!status.IsOk()) return status; if (!std::isfinite(component.Energy) || !std::isfinite(component.Temperature) || component.SeekEnergyWeight < 0.0F || component.SeekShelterWeight < 0.0F) return Status::Error("Utility agent values are invalid"); m_UtilityAgents.InsertOrAssign(entity, component); return Status::Ok(); }
Status World::SetScript(Entity entity, ScriptComponent component) { const auto status = ValidateAlive(entity); if (!status.IsOk()) return status; if (component.Script.empty()) return Status::Error("Script reference cannot be empty"); m_Scripts.InsertOrAssign(entity, std::move(component)); return Status::Ok(); }
Status World::RemoveComponent(Entity entity, const std::string& componentName) { const auto status = ValidateAlive(entity); if (!status.IsOk()) return status; if (componentName == "transform") m_Transforms.Remove(entity); else if (componentName == "renderable") m_Renderables.Remove(entity); else if (componentName == "rigidBody") m_RigidBodies.Remove(entity); else if (componentName == "audioEmitter") m_AudioEmitters.Remove(entity); else if (componentName == "utilityAgent") m_UtilityAgents.Remove(entity); else if (componentName == "script") m_Scripts.Remove(entity); else return Status::Error("Unknown component: " + componentName); return Status::Ok(); }
} // namespace gaia
