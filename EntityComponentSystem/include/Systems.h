/**
 * @file Systems.h
 * @author Ashley Low
 * @brief Defines ECS systems that operate on entities with specific components.
 * @date September 13, 2025
 *
 * Systems define game logic by iterating over entities that have the required components:
 * - MovementSystem: Updates positions based on velocity.
 * - HealthSystem: Decreases health over time and destroys dead entities.
 * - RenderSystem: Outputs entity information to the console.
 */

#pragma once

 // ------------------------------------------------------------------
 // MovementSystem
 // Updates entity positions using their Velocity component.
 // ------------------------------------------------------------------
class MovementSystem {
    EntityManager& mgr; 
public:
    explicit MovementSystem(EntityManager& manager) : mgr(manager) {}

    /**
     * @brief Update positions of all entities with a Velocity component.
     * @param dt Delta time for this tick/frame.
     */
    void update(float dt) {
        // Iterate over all entities with Velocity
        mgr.forEach<Velocity>([&](Entity e, Velocity& v) {
            // If entity has a Position component, update it
            if (auto pos = mgr.getComponent<Position>(e)) {
                pos->x += v.dx * dt;
                pos->y += v.dy * dt;
            }
            });
    }
};

// ------------------------------------------------------------------
// HealthSystem
// Decrements health and destroys entities whose health reaches zero.
// ------------------------------------------------------------------
class HealthSystem {
    EntityManager& mgr; 
public:
    explicit HealthSystem(EntityManager& manager) : mgr(manager) {}

    /**
     * @brief Update health of all entities with a Health component.
     * Reduces HP by 1 per tick and destroys entity if HP reaches 0.
     */
    void update() {
        mgr.forEach<Health>([&](Entity e, Health& h) {
            h.hp = std::max(0, h.hp - 1); // Prevent negative health
            if (h.hp == 0) {
                std::cout << "Destroying entity " << e << " (hp reached 0)\n";
                mgr.destroyEntity(e);
            }
            });
    }
};

// ------------------------------------------------------------------
// RenderSystem
// Prints entity information (Position, Name, Health) to the console.
// ------------------------------------------------------------------
class RenderSystem {
    EntityManager& mgr; 
public:
    explicit RenderSystem(EntityManager& manager) : mgr(manager) {}

    /**
     * @brief Render all entities with a Position component.
     *        Prints Name and Health if the entity has them.
     */
    void render() {
        mgr.forEach<Position>([&](Entity e, Position& p) {
            const Name* n = mgr.getComponent<Name>(e);
            const Health* h = mgr.getComponent<Health>(e);

            std::cout << (n ? n->value : "unnamed") << "[" << e << "]"
                << " pos=(" << p.x << "," << p.y << ")"
                << " hp=" << (h ? h->hp : -1)
                << "\n";
            });
    }
};
