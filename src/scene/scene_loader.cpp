#include "gaia/scene/scene_loader.hpp"
#include "gaia/core/json.hpp"

namespace gaia {
namespace {
Status Schema(const std::string& message) { return Status::Error(ErrorCode::kSchemaError, message); }
const JsonObject* Object(const Json* value) { return value == nullptr ? nullptr : value->object(); }
Result<std::array<float, 3>> Vec3(const Json* value, const std::string& name, std::array<float, 3> fallback) {
    if (value == nullptr) return Result<std::array<float, 3>>::Success(fallback); const JsonArray* a = value->array();
    if (a == nullptr || a->size() != 3) return Result<std::array<float, 3>>::Failure(ErrorCode::kSchemaError, name + " must have 3 numbers");
    std::array<float, 3> out{}; for (size_t i = 0; i < 3; ++i) { const double* n = (*a)[i].number(); if (!n) return Result<std::array<float, 3>>::Failure(ErrorCode::kSchemaError, name + " must contain numbers"); out[i] = static_cast<float>(*n); } return Result<std::array<float, 3>>::Success(out);
}
Status AddEntity(const JsonObject& entity, World& world) {
    const JsonObject* components = Object(Find(entity, "components")); if (!components) return Schema("entity.components must be an object"); Entity e = world.create_entity();
    for (const auto& [name, ignored] : *components) {
        static_cast<void>(ignored);
        if (name != "transform" && name != "velocity" && name != "mesh_renderer" && name != "rigid_body" && name != "audio_source" && name != "script") return Schema("unknown component: " + name);
    }
    if (const JsonObject* c = Object(Find(*components, "transform"))) { auto p = Vec3(Find(*c, "position"), "transform.position", {}); auto r = Vec3(Find(*c, "rotation"), "transform.rotation", {}); auto s = Vec3(Find(*c, "scale"), "transform.scale", {1, 1, 1}); if (!p.ok()) return p.status; if (!r.ok()) return r.status; if (!s.ok()) return s.status; if (auto status = world.add_transform(e, Transform{p.value, r.value, s.value}); !status.ok()) return status; }
    if (const JsonObject* c = Object(Find(*components, "velocity"))) { auto v = Vec3(Find(*c, "meters_per_second"), "velocity.meters_per_second", {}); if (!v.ok()) return v.status; if (auto status = world.add_velocity(e, Velocity{v.value}); !status.ok()) return status; }
    if (const JsonObject* c = Object(Find(*components, "mesh_renderer"))) { const Json* mesh = Find(*c, "mesh"); const Json* material = Find(*c, "material"); if (!mesh || !material || !mesh->string() || !material->string()) return Schema("mesh_renderer requires string mesh and material"); if (auto status = world.add_mesh_renderer(e, MeshRenderer{*mesh->string(), *material->string()}); !status.ok()) return status; }
    if (const JsonObject* c = Object(Find(*components, "rigid_body"))) { const Json* mass = Find(*c, "mass"); if (!mass || !mass->number() || *mass->number() <= 0) return Schema("rigid_body.mass must be positive"); if (auto status = world.add_rigid_body(e, RigidBody{static_cast<float>(*mass->number())}); !status.ok()) return status; }
    if (const JsonObject* c = Object(Find(*components, "audio_source"))) { const Json* clip = Find(*c, "clip"); if (!clip || !clip->string()) return Schema("audio_source.clip must be a string"); if (auto status = world.add_audio_source(e, AudioSource{*clip->string()}); !status.ok()) return status; }
    if (const JsonObject* c = Object(Find(*components, "script"))) { const Json* asset = Find(*c, "asset"); if (!asset || !asset->string()) return Schema("script.asset must be a string"); if (auto status = world.add_script(e, Script{*asset->string()}); !status.ok()) return status; }
    return Status::Ok();
}
}  // namespace
Status SceneLoader::LoadJson(const std::string& source, World& destination) {
    auto parsed = ParseJson(source); if (!parsed.ok()) return parsed.status; const JsonObject* root = parsed.value.object(); if (!root) return Schema("scene root must be an object"); const Json* version = Find(*root, "version"); const Json* entities = Find(*root, "entities");
    if (!version || !version->number() || *version->number() != 1) return Schema("scene version must be 1"); if (!entities || !entities->array()) return Schema("scene.entities must be an array");
    World staged; for (const Json& entity : *entities->array()) { const JsonObject* object = entity.object(); if (!object) return Schema("each entity must be an object"); if (auto status = AddEntity(*object, staged); !status.ok()) return status; }
    destination = std::move(staged); return Status::Ok();
}
}  // namespace gaia
