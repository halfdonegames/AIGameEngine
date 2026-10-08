#pragma once

#include "gaia/core/status.hpp"
#include "gaia/ecs/components.hpp"
#include "gaia/ecs/packed_store.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace gaia {

class World {
public:
    Result<Entity> CreateEntity(std::string name);
    Status DestroyEntity(Entity entity);
    [[nodiscard]] bool IsAlive(Entity entity) const;
    [[nodiscard]] std::size_t EntityCount() const { return m_AliveCount; }

    Status SetTransform(Entity entity, TransformComponent component);
    Status SetRenderable(Entity entity, RenderableComponent component);
    Status SetRigidBody(Entity entity, RigidBodyComponent component);
    Status SetAudioEmitter(Entity entity, AudioEmitterComponent component);
    Status SetUtilityAgent(Entity entity, UtilityAgentComponent component);
    Status SetScript(Entity entity, ScriptComponent component);
    Status RemoveComponent(Entity entity, const std::string& componentName);

    [[nodiscard]] const PackedStore<TransformComponent>& Transforms() const { return m_Transforms; }
    [[nodiscard]] const PackedStore<RenderableComponent>& Renderables() const { return m_Renderables; }
    [[nodiscard]] const PackedStore<RigidBodyComponent>& RigidBodies() const { return m_RigidBodies; }
    [[nodiscard]] const PackedStore<AudioEmitterComponent>& AudioEmitters() const { return m_AudioEmitters; }
    [[nodiscard]] const PackedStore<UtilityAgentComponent>& UtilityAgents() const { return m_UtilityAgents; }
    [[nodiscard]] const PackedStore<ScriptComponent>& Scripts() const { return m_Scripts; }

private:
    struct EntitySlot { uint32_t Generation = 1; bool Alive = false; };
    Status ValidateAlive(Entity entity) const;
    std::vector<EntitySlot> m_Entities;
    std::vector<uint32_t> m_FreeIndices;
    std::size_t m_AliveCount = 0;
    PackedStore<TransformComponent> m_Transforms;
    PackedStore<RenderableComponent> m_Renderables;
    PackedStore<RigidBodyComponent> m_RigidBodies;
    PackedStore<AudioEmitterComponent> m_AudioEmitters;
    PackedStore<UtilityAgentComponent> m_UtilityAgents;
    PackedStore<ScriptComponent> m_Scripts;
};

} // namespace gaia
