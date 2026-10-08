# G.A.I.A. Engine contributor guide

## Scope and design rules

- Keep the engine portable across Windows, macOS, and Ubuntu 24.04+. Build with CMake; do not add platform-specific behavior without an equivalent implementation or a documented capability check.
- Entities are opaque generational IDs. Gameplay state belongs only in packed component stores; never attach stateful behavior to entity objects.
- Public engine APIs must return `Result`/`Status` values for recoverable failure. Do not use exceptions across API boundaries.
- Scene files are the source of truth for authoring. Keep their JSON schema backwards-compatible and validate all untrusted input before modifying a world.
- Renderer, audio, physics, windowing, and scripting are adapters behind interfaces. Core code must remain independently testable and dependency-free.

## Quality gate

1. Format C++ with the repository `.clang-format`.
2. Build with warnings enabled; warnings are errors for engine targets.
3. Add focused unit tests for every behavior change and update `assets/scenes/feature_coverage.json` if a public feature changes.
4. Run `ctest --test-dir build --output-on-failure`.
5. Review `git diff --check` and the staged diff before a commit.

## Safety

- Do not silently discard unknown declarative data.
- Do not use raw owning pointers, global mutable state, or `new`/`delete` in engine code.
- Validate entity liveness on every public ECS operation.
