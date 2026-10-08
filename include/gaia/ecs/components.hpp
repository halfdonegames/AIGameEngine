#pragma once
#include <array>
#include <string>

namespace gaia {
struct Transform { std::array<float, 3> position{}; std::array<float, 3> rotation{}; std::array<float, 3> scale{1, 1, 1}; };
struct Velocity { std::array<float, 3> meters_per_second{}; };
struct MeshRenderer { std::string mesh; std::string material; bool cast_shadows{true}; };
struct RigidBody { float mass{1.0F}; bool kinematic{false}; };
struct AudioSource { std::string clip; float gain{1.0F}; bool looping{false}; };
struct Script { std::string asset; bool enabled{true}; };
}  // namespace gaia
