#include "gaia/scene/scene_loader.hpp"

#include <iostream>

int main(int argc, char** argv) {
    gaia::World world;
    if (argc > 1) {
        const auto status = gaia::SceneLoader::LoadFromFile(argv[1], world);
        if (!status.IsOk()) { std::cerr << "Editor failed to load scene: " << status.Message() << '\n'; return 1; }
    }
    std::cout << "G.A.I.A-I Editor foundation: " << world.EntityCount() << " entities loaded.\n";
    return 0;
}
