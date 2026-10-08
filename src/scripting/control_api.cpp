#include "gaia/scripting/control_api.hpp"
#include "gaia/core/json.hpp"

namespace gaia {
namespace { Result<double> Number(const JsonObject& object, const std::string& key) { const Json* v = Find(object, key); if (!v || !v->number()) return Result<double>::Failure(ErrorCode::kSchemaError, key + " must be a number"); return Result<double>::Success(*v->number()); } }
Result<std::string> ControlApi::execute(const std::string& request) {
    auto parsed = ParseJson(request); if (!parsed.ok()) return Result<std::string>::Failure(parsed.status.code, parsed.status.message); const JsonObject* object = parsed.value.object(); if (!object) return Result<std::string>::Failure(ErrorCode::kSchemaError, "request must be an object"); const Json* command = Find(*object, "command"); if (!command || !command->string()) return Result<std::string>::Failure(ErrorCode::kSchemaError, "command must be a string");
    if (*command->string() == "world.query") return Result<std::string>::Success(ToJson(Json{JsonObject{{"entity_count", Json{static_cast<double>(world_.entity_count())}}, {"transform_count", Json{static_cast<double>(world_.transforms().size())}}}}));
    if (*command->string() == "world.simulate") { auto seconds = Number(*object, "seconds"); if (!seconds.ok()) return Result<std::string>::Failure(seconds.status.code, seconds.status.message); const Status s = world_.simulate(static_cast<float>(seconds.value)); if (!s.ok()) return Result<std::string>::Failure(s.code, s.message); return Result<std::string>::Success("{\"ok\":true}"); }
    if (*command->string() == "entity.create") { Entity e = world_.create_entity(); return Result<std::string>::Success(ToJson(Json{JsonObject{{"index", Json{static_cast<double>(e.index)}}, {"generation", Json{static_cast<double>(e.generation)}}}})); }
    return Result<std::string>::Failure(ErrorCode::kNotFound, "unknown control command");
}
}  // namespace gaia
