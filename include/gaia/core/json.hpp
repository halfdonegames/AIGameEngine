#pragma once

#include "gaia/core/status.hpp"

#include <map>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace gaia {

class JsonValue {
public:
    using Array = std::vector<JsonValue>;
    using Object = std::map<std::string, JsonValue, std::less<>>;
    using Storage = std::variant<std::nullptr_t, bool, double, std::string, Array, Object>;

    JsonValue() : m_Value(nullptr) {}
    explicit JsonValue(Storage value) : m_Value(std::move(value)) {}

    [[nodiscard]] bool IsObject() const;
    [[nodiscard]] bool IsArray() const;
    [[nodiscard]] bool IsString() const;
    [[nodiscard]] bool IsNumber() const;
    [[nodiscard]] bool IsBool() const;
    [[nodiscard]] const Object* AsObject() const;
    [[nodiscard]] const Array* AsArray() const;
    [[nodiscard]] const std::string* AsString() const;
    [[nodiscard]] const double* AsNumber() const;
    [[nodiscard]] const bool* AsBool() const;
    [[nodiscard]] const JsonValue* Find(std::string_view key) const;

private:
    Storage m_Value;
};

Result<JsonValue> ParseJson(const std::string& text);

} // namespace gaia
