#pragma once
#include <map>
#include <string>
#include <variant>
#include <vector>
#include "gaia/core/status.hpp"

namespace gaia {
struct Json;
using JsonArray = std::vector<Json>;
using JsonObject = std::map<std::string, Json, std::less<>>;
struct Json {
    using Value = std::variant<std::nullptr_t, bool, double, std::string, JsonArray, JsonObject>;
    Value value{nullptr};
    [[nodiscard]] bool is_object() const { return std::holds_alternative<JsonObject>(value); }
    [[nodiscard]] bool is_array() const { return std::holds_alternative<JsonArray>(value); }
    [[nodiscard]] const JsonObject* object() const { return std::get_if<JsonObject>(&value); }
    [[nodiscard]] const JsonArray* array() const { return std::get_if<JsonArray>(&value); }
    [[nodiscard]] const std::string* string() const { return std::get_if<std::string>(&value); }
    [[nodiscard]] const double* number() const { return std::get_if<double>(&value); }
    [[nodiscard]] const bool* boolean() const { return std::get_if<bool>(&value); }
};
Result<Json> ParseJson(const std::string& source);
std::string ToJson(const Json& json);
const Json* Find(const JsonObject& object, const std::string& key);
}  // namespace gaia
