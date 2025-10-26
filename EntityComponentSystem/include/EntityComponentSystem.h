/**
 * @file EntityComponentSystem.h
 * @author Ashley low
 * @brief Master header for the Entity-Component-System
 * @date September 13, 2025
 */
#pragma once

 // ------------------------------------------------------------------
 // 1. PRECOMPILED HEADERS/COMMON LIBRARIES 
 // These are standard libraries used frequently throughout the system.
 // ------------------------------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <memory>

// ------------------------------------------------------------------
// 2. ECS MODULES
// Internal headers defining components, entity management, and systems.
// ------------------------------------------------------------------
#include "Component.h"
#include "EntityComponentSystem.h"
#include "EntityManager.h"
#include "Systems.h"