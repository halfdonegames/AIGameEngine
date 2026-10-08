#pragma once

#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace gaia::test {
struct Case { std::string Name; std::function<void()> Function; };
inline std::vector<Case>& Cases() { static std::vector<Case> cases; return cases; }
struct Register { Register(const char* name, std::function<void()> function) { Cases().push_back({name, std::move(function)}); } };
}

#define GAIA_TEST(name) void name(); static gaia::test::Register name##_registration(#name, name); void name()
#define GAIA_REQUIRE(condition) do { if (!(condition)) throw std::runtime_error("Requirement failed: " #condition); } while (false)
