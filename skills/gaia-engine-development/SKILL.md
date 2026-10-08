---
name: gaia-engine-development
description: Build or modify the G.A.I.A. game engine while preserving its strict ECS, declarative scene, portability, and verification requirements.
---

# G.A.I.A. engine development

Use this skill for engine, runtime, editor, scene, or scripting changes in this repository.

Read `AGENTS.md` first. Maintain pure ECS: entities are generational identifiers and mutable gameplay data is stored in packed component arrays. Keep authoring inputs declarative and validate them before world mutation.

For a new public capability, add a focused automated test and extend `assets/scenes/feature_coverage.json` when the capability can be represented declaratively. Keep core code free of platform SDK types; add graphics, audio, physics, windowing, and Lua functionality behind adapters.

Before committing, configure/build/test with CMake, inspect the resulting diff, and report any unavailable optional integration dependency rather than faking support.
