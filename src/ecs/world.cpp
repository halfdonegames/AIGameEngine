#include "gaia/ecs/world.hpp"
#include <cmath>

namespace gaia {
Entity World::create_entity() {
    uint32_t index{};
    if (!free_indices_.empty()) { index = free_indices_.back(); free_indices_.pop_back(); ++generations_[index]; }
    else { index = static_cast<uint32_t>(generations_.size()); generations_.push_back(0); }
    ++live_count_; return Entity{index, generations_[index]};
}
bool World::alive(Entity e) const { return e.index < generations_.size() && generations_[e.index] == e.generation && e.generation % 2 == 0; }
Status World::destroy_entity(Entity e) {
    if (!alive(e)) return Status::Error(ErrorCode::kNotFound, "entity is not alive");
    transforms_.erase(e); velocities_.erase(e); mesh_renderers_.erase(e); rigid_bodies_.erase(e); audio_sources_.erase(e); scripts_.erase(e);
    ++generations_[e.index]; free_indices_.push_back(e.index); --live_count_; return Status::Ok();
}
template <typename T> Status World::add(Entity e, PackedStore<T>& store, T value) {
    if (!alive(e)) return Status::Error(ErrorCode::kNotFound, "entity is not alive");
    if (!store.insert(e, std::move(value))) return Status::Error(ErrorCode::kAlreadyExists, "component already exists");
    return Status::Ok();
}
Status World::add_transform(Entity e, Transform c) { return add(e, transforms_, std::move(c)); }
Status World::add_velocity(Entity e, Velocity c) { return add(e, velocities_, std::move(c)); }
Status World::add_mesh_renderer(Entity e, MeshRenderer c) { return add(e, mesh_renderers_, std::move(c)); }
Status World::add_rigid_body(Entity e, RigidBody c) { return add(e, rigid_bodies_, std::move(c)); }
Status World::add_audio_source(Entity e, AudioSource c) { return add(e, audio_sources_, std::move(c)); }
Status World::add_script(Entity e, Script c) { return add(e, scripts_, std::move(c)); }
Status World::simulate(float seconds) {
    if (!std::isfinite(seconds) || seconds < 0.0F) return Status::Error(ErrorCode::kInvalidArgument, "delta_seconds must be finite and non-negative");
    for (const Entity e : velocities_.entities()) { if (Transform* t = transforms_.get(e)) { const Velocity* v = velocities_.get(e); for (size_t i = 0; i < 3; ++i) t->position[i] += v->meters_per_second[i] * seconds; } }
    return Status::Ok();
}
}  // namespace gaia
