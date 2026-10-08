#include "gaia/scene/scene_loader.hpp"
#include "test_framework.hpp"

#include <fstream>
#include <sstream>

GAIA_TEST(FeatureCoverageSceneLoadsEveryComponent) {
    std::ifstream input("assets/scenes/feature_coverage.json");
    if (!input) input.open("../assets/scenes/feature_coverage.json");
    std::ostringstream text; text << input.rdbuf();
    gaia::World world;
    GAIA_REQUIRE(gaia::SceneLoader::LoadFromText(text.str(), world).IsOk());
    GAIA_REQUIRE(world.EntityCount() == 1);
    GAIA_REQUIRE(world.Transforms().Size() == 1 && world.Renderables().Size() == 1 && world.RigidBodies().Size() == 1);
    GAIA_REQUIRE(world.AudioEmitters().Size() == 1 && world.UtilityAgents().Size() == 1 && world.Scripts().Size() == 1);
}

GAIA_TEST(SceneLoaderRejectsMalformedAndInvalidSceneData) {
    gaia::World world;
    GAIA_REQUIRE(gaia::SceneLoader::LoadFromText("{\"entities\":[{\"name\":\"kept\",\"components\":{}}]}", world).IsOk());
    const auto entitiesBeforeFailure = world.EntityCount();
    GAIA_REQUIRE(!gaia::SceneLoader::LoadFromText("{", world).IsOk());
    GAIA_REQUIRE(!gaia::SceneLoader::LoadFromText("{\"entities\":[{\"name\":\"bad\",\"components\":{\"renderable\":{\"mesh\":\"\",\"material\":\"m\"}}}]}", world).IsOk());
    GAIA_REQUIRE(world.EntityCount() == entitiesBeforeFailure);
}
