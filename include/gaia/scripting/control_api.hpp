#pragma once
#include <string>
#include "gaia/core/status.hpp"
#include "gaia/ecs/world.hpp"
namespace gaia {
class ControlApi {
  public:
    explicit ControlApi(World& world) : world_(world) {}
    Result<std::string> execute(const std::string& request_json);
  private: World& world_;
};
}  // namespace gaia
