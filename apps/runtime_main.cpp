#include <iostream>
#include "gaia/ecs/world.hpp"
int main() { gaia::World world; std::cout << "G.A.I.A. runtime initialized (entities=" << world.entity_count() << ")\n"; return 0; }
