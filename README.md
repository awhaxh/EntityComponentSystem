# EntityComponentSystem

A modular **Entity–Component–System (ECS)** framework written in modern **C++20**.  
This project demonstrates a clean, extensible architecture for managing entities and their associated data and behavior — a core pattern used in game engines and simulations.

---

## Overview

The `EntityComponentSystem` project implements a lightweight, data-oriented **ECS architecture** designed to separate *data (components)* from *behavior (systems)*.  

Entities are simple identifiers, Components store pure data, and Systems operate over entities that possess specific component sets.

This framework allows for **flexible composition**, **type-safe component management**, and **efficient entity iteration**, making it ideal for real-time simulations and game prototypes.

---

## Project Structure
```                  
├── EntityComponentSystem               (Main ECS library and demo)
│   ├── CMakeLists.txt                  (Build logic for EntityComponentSystem)
│   ├── include                    
│   │   ├── Component.h                 (ECS component definitions)
│   │   ├── EntityComponentSystem.h     (Main library interface & includes precompiled header directives)
│   │   ├── EntityManager.h             (Entity and component management)
│   │   └── Systems.h                   (ECS system definitions)
│   └── src                        
│       └── EntityComponentSystem.cpp   (Core library implementation)
├── CMakeLists.txt                      (Top-level project configuration)
├── CMakePresets.json
├── LICENSE                             (Project license)
└── README.md                           (Project documentation)
```
---

## Build Instructions

### Prerequisites

* CMake 3.12+
* C++20-compatible compiler (MSVC, Clang, or GCC)

### Steps

Navigate to the project root and run the following commands:

```bash
# Create a build directory
mkdir build
cd build

# Generate platform-specific build files (e.g., Visual Studio solution or Makefiles)
cmake ..

# Build the executable (EntityComponentSystem)
cmake --build .

# Navigate to the executable folder
cd EntityComponentSystem

# Run the demo
./EntityComponentSystem
```

---

## ECS Architecture
### 1. Entities
* Entities are lightweight integer IDs that act as containers for components.
* They have no data or behavior themselves — they simply link components together.
```
Entity e = mgr.createEntity();
```

### 2. Components
* Components store raw data only — no logic.
* Examples include Position, Velocity, Health, and Name.
```
struct Position { float x, y; };
struct Velocity { float dx, dy; };
struct Health   { int hp; };
struct Name     { std::string value; };
```

### 3. Systems
* Systems define logic and operate on all entities with matching components.
* Each system demonstrates a specific responsibility:

| System        | Purpose                                       |
| ----------    | ----------------------------------------------|
| MovementSystem| Updates positions based on velocity           |
| HealthSystem  | Decrements health and removes dead entities   |
| RenderSystem  | Prints entity state to the console            |

Example:
```
MovementSystem movement(mgr);
movement.update(deltaTime);
```

---
## Example Usage:
The included main() function demonstrates:
* Creating several entities with various components
* Running a 10-tick simulation loop
* Updating movement, health, and rendering each frame
* Recycling entity IDs when entities are destroyed

## Example Output:
```
Tick 0 ----------------
Entity_0[0] pos=(0,0) hp=99
Entity_1[1] pos=(1.1,0.05) hp=89
Entity_2[2] pos=(2.2,0.1) hp=79
Entity_3[3] pos=(3.3,0.15) hp=69
Entity_4[4] pos=(4.4,0.2) hp=59
-------------------------
```

---

## Customization

You can easily extend this ECS by:

* Adding new components (e.g., Inventory, AIState, RenderMesh)
* Implementing new systems (e.g., CollisionSystem, CombatSystem)
* Integrating with graphics, physics, or AI modules

Each new system can iterate over entities that have specific component combinations, providing a highly modular design.

---

## Author

Ashley Low

Project developed as part of a programming portfolio.

[LinkedIn](https://www.linkedin.com/in/ashleyllj)