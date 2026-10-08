#include "gaia/scripting/control_api.hpp"
#include "test_framework.hpp"

GAIA_TEST(ControlApiCreatesMutatesAndDestroysEntities) {
    gaia::World world; gaia::ControlApi api(world);
    const auto entity = api.Execute("spawn Agent Alpha"); GAIA_REQUIRE(entity.IsOk());
    const auto id = entity.Value();
    GAIA_REQUIRE(api.Execute("transform " + std::to_string(id.Index) + " " + std::to_string(id.Generation) + " 1 2 3 0 90 0 1 1 1").IsOk());
    GAIA_REQUIRE(world.Transforms().Contains(id));
    GAIA_REQUIRE(api.Execute("remove " + std::to_string(id.Index) + " " + std::to_string(id.Generation) + " transform").IsOk());
    GAIA_REQUIRE(!world.Transforms().Contains(id));
    GAIA_REQUIRE(api.Execute("destroy " + std::to_string(id.Index) + " " + std::to_string(id.Generation)).IsOk());
    GAIA_REQUIRE(!api.Execute("destroy " + std::to_string(id.Index) + " " + std::to_string(id.Generation)).IsOk());
}
