#include "gaia/scene/scene_loader.hpp"

#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) { std::cerr << "Usage: GaiaRuntime <scene.json>\n"; return 2; }
    gaia::World world;
    const auto status = gaia::SceneLoader::LoadFromFile(argv[1], world);
    if (!status.IsOk()) { std::cerr << "Runtime failed to load scene: " << status.Message() << '\n'; return 1; }
    std::cout << "G.A.I.A-I Runtime: " << world.EntityCount() << " entities loaded.\n";
    return 0;
}
