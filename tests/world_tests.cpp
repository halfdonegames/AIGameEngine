#include "gaia/ecs/world.hpp"
#include "test_framework.hpp"

GAIA_TEST(WorldRejectsStaleEntitiesAndReusesSlotsSafely) {
    gaia::World world;
    const auto original = world.CreateEntity("original");
    GAIA_REQUIRE(original.IsOk());
    GAIA_REQUIRE(world.DestroyEntity(original.Value()).IsOk());
    const auto replacement = world.CreateEntity("replacement");
    GAIA_REQUIRE(replacement.IsOk());
    GAIA_REQUIRE(replacement.Value().Index == original.Value().Index);
    GAIA_REQUIRE(replacement.Value().Generation != original.Value().Generation);
    GAIA_REQUIRE(!world.SetScript(original.Value(), {"bad.lua"}).IsOk());
}

GAIA_TEST(WorldRejectsInvalidComponentData) {
    gaia::World world; const auto entity = world.CreateEntity("entity"); GAIA_REQUIRE(entity.IsOk());
    GAIA_REQUIRE(!world.SetRenderable(entity.Value(), {"", "mat"}).IsOk());
    GAIA_REQUIRE(!world.SetRigidBody(entity.Value(), {-1.0F, 1.0F, 1.0F, 1.0F}).IsOk());
    GAIA_REQUIRE(!world.SetTransform(entity.Value(), {{0, 0, 0}, {0, 0, 0}, {1, 0, 1}}).IsOk());
}
