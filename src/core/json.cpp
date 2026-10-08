#include "gaia/core/json.hpp"
#include <cctype>
#include <charconv>
#include <sstream>

namespace gaia {
namespace {
class Parser {
  public:
    explicit Parser(const std::string& input) : input_(input) {}
    Result<Json> parse() { skip(); auto value = parse_value(); if (!value.ok()) return value; skip(); return pos_ == input_.size() ? value : Result<Json>::Failure(ErrorCode::kParseError, "unexpected trailing JSON content"); }
  private:
    void skip() { while (pos_ < input_.size() && std::isspace(static_cast<unsigned char>(input_[pos_]))) ++pos_; }
    bool take(char c) { skip(); if (pos_ < input_.size() && input_[pos_] == c) { ++pos_; return true; } return false; }
    Result<Json> parse_value() {
        skip(); if (pos_ == input_.size()) return Result<Json>::Failure(ErrorCode::kParseError, "unexpected end of JSON");
        if (input_[pos_] == '{') return parse_object(); if (input_[pos_] == '[') return parse_array(); if (input_[pos_] == '"') { auto s = parse_string(); return s.ok() ? Result<Json>::Success(Json{s.value}) : Result<Json>::Failure(s.status.code, s.status.message); }
        if (input_.compare(pos_, 4, "true") == 0) { pos_ += 4; return Result<Json>::Success(Json{true}); }
        if (input_.compare(pos_, 5, "false") == 0) { pos_ += 5; return Result<Json>::Success(Json{false}); }
        if (input_.compare(pos_, 4, "null") == 0) { pos_ += 4; return Result<Json>::Success(Json{nullptr}); }
        return parse_number();
    }
    Result<std::string> parse_string() {
        if (!take('"')) return Result<std::string>::Failure(ErrorCode::kParseError, "expected string"); std::string out;
        while (pos_ < input_.size() && input_[pos_] != '"') { if (input_[pos_] == '\\') { ++pos_; if (pos_ == input_.size()) return Result<std::string>::Failure(ErrorCode::kParseError, "unterminated escape"); const char c = input_[pos_++]; if (c == '"' || c == '\\' || c == '/') out += c; else if (c == 'n') out += '\n'; else if (c == 't') out += '\t'; else return Result<std::string>::Failure(ErrorCode::kParseError, "unsupported string escape"); } else { out += input_[pos_++]; } }
        if (pos_ == input_.size()) return Result<std::string>::Failure(ErrorCode::kParseError, "unterminated string"); ++pos_; return Result<std::string>::Success(std::move(out));
    }
    Result<Json> parse_number() { skip(); const size_t begin = pos_; while (pos_ < input_.size() && (std::isdigit(static_cast<unsigned char>(input_[pos_])) || input_[pos_] == '-' || input_[pos_] == '+' || input_[pos_] == '.' || input_[pos_] == 'e' || input_[pos_] == 'E')) ++pos_; double value{}; const auto [end, error] = std::from_chars(input_.data() + begin, input_.data() + pos_, value); if (error != std::errc{} || end != input_.data() + pos_) return Result<Json>::Failure(ErrorCode::kParseError, "invalid number"); return Result<Json>::Success(Json{value}); }
    Result<Json> parse_array() { take('['); JsonArray out; skip(); if (take(']')) return Result<Json>::Success(Json{std::move(out)}); while (true) { auto v = parse_value(); if (!v.ok()) return v; out.push_back(std::move(v.value)); if (take(']')) return Result<Json>::Success(Json{std::move(out)}); if (!take(',')) return Result<Json>::Failure(ErrorCode::kParseError, "expected array comma"); } }
    Result<Json> parse_object() { take('{'); JsonObject out; skip(); if (take('}')) return Result<Json>::Success(Json{std::move(out)}); while (true) { auto key = parse_string(); if (!key.ok()) return Result<Json>::Failure(key.status.code, key.status.message); if (!take(':')) return Result<Json>::Failure(ErrorCode::kParseError, "expected object colon"); auto value = parse_value(); if (!value.ok()) return value; if (!out.emplace(std::move(key.value), std::move(value.value)).second) return Result<Json>::Failure(ErrorCode::kParseError, "duplicate object key"); if (take('}')) return Result<Json>::Success(Json{std::move(out)}); if (!take(',')) return Result<Json>::Failure(ErrorCode::kParseError, "expected object comma"); } }
    const std::string& input_; size_t pos_{};
};
std::string Escape(const std::string& s) { std::string out; for (char c : s) { if (c == '"' || c == '\\') { out += '\\'; out += c; } else if (c == '\n') out += "\\n"; else out += c; } return out; }
}  // namespace
Result<Json> ParseJson(const std::string& source) { return Parser(source).parse(); }
const Json* Find(const JsonObject& object, const std::string& key) { const auto found = object.find(key); return found == object.end() ? nullptr : &found->second; }
std::string ToJson(const Json& j) { if (std::holds_alternative<std::nullptr_t>(j.value)) return "null"; if (const auto* b = j.boolean()) return *b ? "true" : "false"; if (const auto* n = j.number()) { std::ostringstream o; o << *n; return o.str(); } if (const auto* s = j.string()) return "\"" + Escape(*s) + "\""; if (const auto* a = j.array()) { std::string o = "["; for (size_t i = 0; i < a->size(); ++i) { if (i) o += ','; o += ToJson((*a)[i]); } return o + ']'; } std::string o = "{"; bool first = true; for (const auto& [k, v] : *j.object()) { if (!first) o += ','; first = false; o += "\"" + Escape(k) + "\":" + ToJson(v); } return o + '}'; }
}  // namespace gaia
