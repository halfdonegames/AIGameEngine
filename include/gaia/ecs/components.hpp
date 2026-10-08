#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace gaia {

struct TransformComponent {
    std::array<float, 3> Position {0.0F, 0.0F, 0.0F};
    std::array<float, 3> RotationDegrees {0.0F, 0.0F, 0.0F};
    std::array<float, 3> Scale {1.0F, 1.0F, 1.0F};
};

struct RenderableComponent { std::string Mesh; std::string Material; };
struct RigidBodyComponent { float Mass = 1.0F; float GravityScale = 1.0F; float AtmosphericDensity = 1.0F; float Viscosity = 1.0F; };
struct AudioEmitterComponent { std::string Clip; float Gain = 1.0F; bool Loop = false; };
struct UtilityAgentComponent { float Energy = 1.0F; float Temperature = 0.5F; float SeekEnergyWeight = 1.0F; float SeekShelterWeight = 1.0F; };
struct ScriptComponent { std::string Script; };

} // namespace gaia
