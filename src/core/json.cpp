#include "gaia/core/json.hpp"

#include <cctype>
#include <charconv>
#include <cstdlib>
#include <string_view>
#include <utility>

namespace gaia {
bool JsonValue::IsObject() const { return std::holds_alternative<Object>(m_Value); }
bool JsonValue::IsArray() const { return std::holds_alternative<Array>(m_Value); }
bool JsonValue::IsString() const { return std::holds_alternative<std::string>(m_Value); }
bool JsonValue::IsNumber() const { return std::holds_alternative<double>(m_Value); }
bool JsonValue::IsBool() const { return std::holds_alternative<bool>(m_Value); }
const JsonValue::Object* JsonValue::AsObject() const { return std::get_if<Object>(&m_Value); }
const JsonValue::Array* JsonValue::AsArray() const { return std::get_if<Array>(&m_Value); }
const std::string* JsonValue::AsString() const { return std::get_if<std::string>(&m_Value); }
const double* JsonValue::AsNumber() const { return std::get_if<double>(&m_Value); }
const bool* JsonValue::AsBool() const { return std::get_if<bool>(&m_Value); }
const JsonValue* JsonValue::Find(std::string_view key) const { const auto* object = AsObject(); if (object == nullptr) return nullptr; const auto iterator = object->find(key); return iterator == object->end() ? nullptr : &iterator->second; }

namespace {
class Parser {
public:
    explicit Parser(const std::string& input) : m_Input(input) {}
    Result<JsonValue> Parse() { SkipWhitespace(); auto value = ParseValue(); if (!value.IsOk()) return value; SkipWhitespace(); return m_Position == m_Input.size() ? value : Result<JsonValue>::Failure("Unexpected trailing JSON content"); }
private:
    Result<JsonValue> ParseValue() { SkipWhitespace(); if (m_Position >= m_Input.size()) return Result<JsonValue>::Failure("Unexpected end of JSON"); const char c = m_Input[m_Position]; if (c == '{') return ParseObject(); if (c == '[') return ParseArray(); if (c == '"') return ParseString(); if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) return ParseNumber(); if (m_Input.compare(m_Position, 4, "true") == 0) { m_Position += 4; return Result<JsonValue>::Success(JsonValue(true)); } if (m_Input.compare(m_Position, 5, "false") == 0) { m_Position += 5; return Result<JsonValue>::Success(JsonValue(false)); } if (m_Input.compare(m_Position, 4, "null") == 0) { m_Position += 4; return Result<JsonValue>::Success(JsonValue()); } return Result<JsonValue>::Failure("Invalid JSON token"); }
    Result<JsonValue> ParseObject() { JsonValue::Object object; ++m_Position; SkipWhitespace(); if (Consume('}')) return Result<JsonValue>::Success(JsonValue(std::move(object))); while (true) { if (m_Position >= m_Input.size() || m_Input[m_Position] != '"') return Result<JsonValue>::Failure("Expected object key"); auto key = ParseString(); if (!key.IsOk()) return key; SkipWhitespace(); if (!Consume(':')) return Result<JsonValue>::Failure("Expected ':' after object key"); auto value = ParseValue(); if (!value.IsOk()) return value; const auto* text = key.Value().AsString(); if (!object.emplace(*text, std::move(value.Value())).second) return Result<JsonValue>::Failure("Duplicate object key"); SkipWhitespace(); if (Consume('}')) break; if (!Consume(',')) return Result<JsonValue>::Failure("Expected ',' in object"); SkipWhitespace(); } return Result<JsonValue>::Success(JsonValue(std::move(object))); }
    Result<JsonValue> ParseArray() { JsonValue::Array array; ++m_Position; SkipWhitespace(); if (Consume(']')) return Result<JsonValue>::Success(JsonValue(std::move(array))); while (true) { auto value = ParseValue(); if (!value.IsOk()) return value; array.push_back(std::move(value.Value())); SkipWhitespace(); if (Consume(']')) break; if (!Consume(',')) return Result<JsonValue>::Failure("Expected ',' in array"); SkipWhitespace(); } return Result<JsonValue>::Success(JsonValue(std::move(array))); }
    Result<JsonValue> ParseString() { std::string value; ++m_Position; while (m_Position < m_Input.size() && m_Input[m_Position] != '"') { const char c = m_Input[m_Position++]; if (static_cast<unsigned char>(c) < 0x20) return Result<JsonValue>::Failure("Control character in JSON string"); if (c == '\\') { if (m_Position >= m_Input.size()) return Result<JsonValue>::Failure("Incomplete JSON escape"); const char escaped = m_Input[m_Position++]; if (escaped == '"' || escaped == '\\' || escaped == '/') value += escaped; else if (escaped == 'n') value += '\n'; else if (escaped == 'r') value += '\r'; else if (escaped == 't') value += '\t'; else return Result<JsonValue>::Failure("Unsupported JSON escape"); } else value += c; } if (!Consume('"')) return Result<JsonValue>::Failure("Unterminated JSON string"); return Result<JsonValue>::Success(JsonValue(std::move(value))); }
    Result<JsonValue> ParseNumber() { const char* begin = m_Input.data() + m_Position; char* end = nullptr; const double value = std::strtod(begin, &end); if (end == begin) return Result<JsonValue>::Failure("Invalid JSON number"); m_Position += static_cast<std::size_t>(end - begin); return Result<JsonValue>::Success(JsonValue(value)); }
    bool Consume(char expected) { if (m_Position < m_Input.size() && m_Input[m_Position] == expected) { ++m_Position; return true; } return false; }
    void SkipWhitespace() { while (m_Position < m_Input.size() && std::isspace(static_cast<unsigned char>(m_Input[m_Position]))) ++m_Position; }
    const std::string& m_Input; std::size_t m_Position = 0;
};
}
Result<JsonValue> ParseJson(const std::string& text) { return Parser(text).Parse(); }
} // namespace gaia
