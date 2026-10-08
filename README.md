# G.A.I.A. Engine

G.A.I.A. is a portable, data-oriented 3D game-engine foundation. It treats worlds as declarative JSON and gameplay state as packed ECS component arrays, so both people and AI tools can author scenes predictably.

## Current foundation

- Generational entity IDs and packed sparse-set component stores.
- Pure data components: transform, velocity, mesh renderer, rigid body, audio source, and script.
- Atomic, validated JSON scene loading and deterministic simulation updates.
- Structured LLM control API for querying and changing worlds.
- A feature-coverage scene and portable unit-test suite.
- Executable editor/runtime hosts ready to be connected to GLFW, NVRHI/Vulkan, miniaudio, a physics adapter, and Lua.

## Build and test

```powershell
cmake -S . -B build -DGAIA_BUILD_TESTS=ON
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

The default build deliberately has no network-time dependency fetches. Production integrations are feature-gated so the deterministic ECS, authoring pipeline, and tests are always buildable first.

## Scene format

See `assets/scenes/feature_coverage.json`. Scene loading is transactional: a malformed file leaves the active world untouched. Component data is explicit JSON designed for tooling and AI agents rather than editor clicks.

## Integration roadmap

The adapter seam is intentional. GLFW/NVRHI (Vulkan), glm, miniaudio, Lua, and a physics implementation should be pinned through a lockfile/package manager and implemented in `platform/` without leaking their types into `gaia-core`.
