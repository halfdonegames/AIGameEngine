#include "test_framework.hpp"
#include "gaia/ecs/world.hpp"
TEST(EntityReuseInvalidatesOldHandle) { gaia::World w; auto first = w.create_entity(); CHECK(w.alive(first)); CHECK(w.destroy_entity(first).ok()); CHECK(!w.alive(first)); auto second = w.create_entity(); CHECK(second.index == first.index); CHECK(second.generation != first.generation); CHECK(w.alive(second)); }
TEST(PackedComponentsAndSimulation) { gaia::World w; auto e = w.create_entity(); CHECK(w.add_transform(e, {}).ok()); CHECK(w.add_velocity(e, {{2.0F, 0.0F, -1.0F}}).ok()); CHECK(w.simulate(0.5F).ok()); CHECK(w.transform(e)->position[0] == 1); CHECK(w.transform(e)->position[2] == -0.5F); CHECK(!w.add_transform(e, {}).ok()); CHECK(!w.simulate(-1).ok()); }
TEST(DestroyRemovesAllComponents) { gaia::World w; auto e = w.create_entity(); CHECK(w.add_transform(e, {}).ok()); CHECK(w.destroy_entity(e).ok()); CHECK(w.transforms().size() == 0); CHECK(w.transform(e) == nullptr); }
