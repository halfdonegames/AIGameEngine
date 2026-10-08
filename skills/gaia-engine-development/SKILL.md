---
name: gaia-engine-development
description: Implement or review G.A.I.A-I engine code, scenes, scripting commands, ECS systems, or build targets while preserving the project’s data-oriented and test-first architecture.
---

# G.A.I.A-I engine development

Use this skill for work inside this repository that changes engine behavior or authored scene data.

- Read `AGENTS.md` first and preserve its portability, data-oriented, and validation rules.
- Treat JSON scenes and the scripting control surface as public contracts. Extend them compatibly and cover malformed as well as valid input in tests.
- Keep the core free of editor, renderer, and platform implementation dependencies. Add interfaces and capability states before backend-specific code.
- Run the CMake build and the full test suite after changes. Include the feature-coverage scene in the test path whenever the schema changes.
- Before a commit, review the complete staged diff for style, ownership, bounds validation, stale-entity behavior, deterministic ordering, and error propagation.
