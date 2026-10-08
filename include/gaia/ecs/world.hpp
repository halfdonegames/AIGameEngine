#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include "gaia/core/status.hpp"
#include "gaia/ecs/components.hpp"
#include "gaia/ecs/entity.hpp"
#include "gaia/ecs/packed_store.hpp"

namespace gaia {
class World {
  public:
    Entity create_entity();
    Status destroy_entity(Entity entity);
    [[nodiscard]] bool alive(Entity entity) const;
    Status add_transform(Entity, Transform); Status add_velocity(Entity, Velocity);
    Status add_mesh_renderer(Entity, MeshRenderer); Status add_rigid_body(Entity, RigidBody);
    Status add_audio_source(Entity, AudioSource); Status add_script(Entity, Script);
    Transform* transform(Entity e) { return transforms_.get(e); } const Transform* transform(Entity e) const { return transforms_.get(e); }
    Velocity* velocity(Entity e) { return velocities_.get(e); } const Velocity* velocity(Entity e) const { return velocities_.get(e); }
    [[nodiscard]] const PackedStore<Transform>& transforms() const { return transforms_; }
    [[nodiscard]] const PackedStore<MeshRenderer>& mesh_renderers() const { return mesh_renderers_; }
    [[nodiscard]] const PackedStore<RigidBody>& rigid_bodies() const { return rigid_bodies_; }
    [[nodiscard]] const PackedStore<AudioSource>& audio_sources() const { return audio_sources_; }
    [[nodiscard]] const PackedStore<Script>& scripts() const { return scripts_; }
    [[nodiscard]] size_t entity_count() const { return live_count_; }
    Status simulate(float delta_seconds);
  private:
    template <typename T> Status add(Entity e, PackedStore<T>& store, T value);
    std::vector<uint32_t> generations_; std::vector<uint32_t> free_indices_; size_t live_count_{};
    PackedStore<Transform> transforms_; PackedStore<Velocity> velocities_; PackedStore<MeshRenderer> mesh_renderers_;
    PackedStore<RigidBody> rigid_bodies_; PackedStore<AudioSource> audio_sources_; PackedStore<Script> scripts_;
};
}  // namespace gaia
