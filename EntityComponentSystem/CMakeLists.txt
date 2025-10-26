# ==============================================================================
# Target: EntityComponentSystem (Executable) - Main application build.
# This file compiles the core source and sets up necessary includes.
# ==============================================================================

# Minimum required CMake version.
cmake_minimum_required(VERSION 3.12)

# Set the project name for the executable build.
project(EntityComponentSystem LANGUAGES CXX)

# --- Compiler Standards Configuration ---
# Enforce C++20 standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Disable compiler extensions for better portability.
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Source File Collection ---
# Recursively find all implementation files (.cpp) in the 'src' directory.
file(GLOB_RECURSE ECS_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# --- Executable Definition ---
# Create the executable from the collected sources (ECS_SOURCES must contain a main() function).
add_executable(EntityComponentSystem ${ECS_SOURCES})

# --- Include Directories Configuration ---
# Add the 'include' folder to the header search path. Use PRIVATE scope as this is the final target.
target_include_directories(EntityComponentSystem PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)