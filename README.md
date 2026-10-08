# G.A.I.A-I

G.A.I.A-I is an AI-first, data-oriented 3D game-engine foundation. Its core is a portable C++20 static library with a strict generational-entity ECS, human/agent-readable scenes, and a transactional command API suitable for editor automation. Separate editor and export-runtime executables consume the same core.

## Current foundation

- Packed, deterministic ECS stores for transforms, renderable mesh/material references, rigid-body parameters, audio emitters, utility-AI agents, and scripts.
- Validated JSON scenes; the checked-in feature-coverage scene exercises every current component and scripting command.
- An automation control API that can create/destroy entities and add, update, and remove components without UI automation.
- Native CMake build and test presets. The foundation intentionally has no fake renderer: Vulkan/NVRHI, GLFW, GLM, miniaudio, Lua/Rust, physics, glTF, and editor UI adapters are planned backend integrations with explicit interfaces, capability reporting, and tests before activation.

## Build and test

```powershell
cmake --preset default
cmake --build --preset default
ctest --test-dir build/default --output-on-failure
```

## Scene contract

Scenes are UTF-8 JSON. An entity has a positive stable `id`, a display `name`, and a `components` object. Component arrays are sorted by entity ID by the ECS, making snapshots deterministic. World-scale and simulation values are data fields rather than engine constants.

This repository is a solid, tested platform for the larger engine roadmap—not a claim that the unimplemented renderer, MPM fluid solver, GPU compute simulation, Rust bridge, or production editor already exist.
