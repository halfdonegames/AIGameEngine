# G.A.I.A-I development guide

## Scope and architecture

- Keep `GaiaCore` a portable, warning-clean static C++ library. Platform code belongs behind an interface.
- Entities are opaque generational IDs. Store component data only in packed, contiguous stores; never attach behavior or ownership graphs to entities.
- Scene files are the source of truth for authored worlds. Keep them deterministic, UTF-8 JSON, reviewable, and round-trippable.
- Public headers use `gaia` namespaces, PascalCase types/functions, `m_` private members, and `s_` static variables, following the requested Hazel-style conventions.
- Treat all file data and script commands as untrusted. Validate every numeric range, identifier, duplicate entity, and component requirement before mutating a world.

## Change discipline

- Write tests before or alongside behavior changes. Add coverage for successful and rejected inputs, stale IDs, and state atomicity where applicable.
- Run `cmake --preset default`, `cmake --build --preset default`, and `ctest --preset default --output-on-failure` before a commit.
- Do not make a renderer, physics system, scripting host, or external dependency mandatory until it has a platform-neutral boundary, failure path, and automated coverage.
- Keep editor-only behavior out of exported runtime targets. Exported builds must load scene data without editor dependencies.

## Data and performance

- Prefer structure-of-arrays/packed component stores, explicit data views, and batch APIs. Do not introduce per-entity heap allocation into hot paths.
- Any GPU-driven feature needs a CPU fallback path and capability reporting.
- Physics and simulation parameters must be dimensionless/configurable scene data; do not embed world constants in systems.

## Completion gate

Before committing: inspect the diff, run formatting and tests, ensure feature coverage scene still validates, and document any intentional deferred capability in `README.md`.
