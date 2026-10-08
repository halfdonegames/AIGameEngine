#include <fstream>
#include <iterator>
#include "test_framework.hpp"
#include "gaia/scene/scene_loader.hpp"
TEST(FeatureCoverageSceneLoadsEveryComponent) { std::ifstream file(std::string(GAIA_SOURCE_DIR) + "/assets/scenes/feature_coverage.json"); std::string source((std::istreambuf_iterator<char>(file)), {}); gaia::World world; CHECK(gaia::SceneLoader::LoadJson(source, world).ok()); CHECK(world.entity_count() == 1); CHECK(world.transforms().size() == 1); CHECK(world.mesh_renderers().size() == 1); CHECK(world.rigid_bodies().size() == 1); CHECK(world.audio_sources().size() == 1); CHECK(world.scripts().size() == 1); }
TEST(SceneLoadIsTransactional) { gaia::World world; world.create_entity(); const auto status = gaia::SceneLoader::LoadJson("{\"version\":1,\"entities\":[{\"components\":{\"rigid_body\":{\"mass\":-1}}}]}", world); CHECK(!status.ok()); CHECK(world.entity_count() == 1); }
TEST(SceneRejectsUnknownVersion) { gaia::World world; CHECK(!gaia::SceneLoader::LoadJson("{\"version\":2,\"entities\":[]}", world).ok()); }
TEST(SceneRejectsUnknownComponents) { gaia::World world; CHECK(!gaia::SceneLoader::LoadJson("{\"version\":1,\"entities\":[{\"components\":{\"unknown\":{}}}]}", world).ok()); }
