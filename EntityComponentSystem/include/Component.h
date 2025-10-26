/**
 * @file Component.h
 * @author Ashley Low
 * @brief Defines all ECS components for the Entity-Component-System framework.
 * @date September 13, 2025
 */
#pragma once

/**
 * @brief Component representing an entity's 2D position.
 */
struct Position {
    float x = 0.0f;
    float y = 0.0f;
};

/**
 * @brief Component representing an entity's 2D velocity.
 */
struct Velocity {
    float dx = 0.0f; 
    float dy = 0.0f; 
};

/**
 * @brief Component representing an entity's health points.
 */
struct Health {
    int hp = 100; 
};

/**
 * @brief Component representing an entity's name.
 */
struct Name {
    std::string value; 
};