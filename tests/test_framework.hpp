#pragma once
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
namespace gaia::test { struct Case { std::string name; std::function<void()> run; }; inline std::vector<Case>& Cases() { static std::vector<Case> cases; return cases; } struct Registrar { Registrar(const char* name, std::function<void()> fn) { Cases().push_back({name, std::move(fn)}); } }; inline void Check(bool value, const char* expression, const char* file, int line) { if (!value) { std::cerr << file << ':' << line << ": failed: " << expression << '\n'; std::exit(1); } } }
#define TEST(name) void name(); static ::gaia::test::Registrar name##_registrar(#name, name); void name()
#define CHECK(expression) ::gaia::test::Check((expression), #expression, __FILE__, __LINE__)
