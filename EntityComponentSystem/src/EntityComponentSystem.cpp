/**
 * @file EntityComponentSystem.cpp
 * @author Ashley low
 * @brief Main simulation file demonstrating the Entity Component System in action.
 * @date September 13, 2025
 *
 */

#include "EntityComponentSystem.h"

using namespace std;

int main() {
    EntityManager mgr;

    // spawn a few entities
    for (int i = 0; i < 5; ++i) {
        Entity e = mgr.createEntity();
        mgr.addComponent<Position>(e, float(i * 1.0f), 0.0f);
        mgr.addComponent<Velocity>(e, 0.1f * i, 0.05f * i);
        mgr.addComponent<Health>(e, 100 - i * 10);
        mgr.addComponent<Name>(e, Name{ "Entity_" + std::to_string(e) });
    }

    MovementSystem movementSystem(mgr);
    HealthSystem healthSystem(mgr);
    RenderSystem renderSystem(mgr);

    const int ticks = 10;
    const float dt = 1.0f;

    for (int t = 0; t < ticks; ++t) {
        std::cout << "Tick " << t << " ----------------\n";
        movementSystem.update(dt);
        healthSystem.update();
        renderSystem.render();
        std::cout << "-------------------------\n";
    }

    // create another entity to show ID recycling
    Entity e = mgr.createEntity();
    mgr.addComponent<Name>(e, Name{ "recycled" });
    std::cout << "New entity created with id: " << e << "\n";

    return 0;
}

