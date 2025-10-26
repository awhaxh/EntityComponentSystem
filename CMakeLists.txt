# ==============================================================================
# Project: EntityComponentSystem (Top-Level Configuration)
# Sets global standards and includes the main EntityComponentSystem sub-directory.
# ==============================================================================

# Minimum required CMake version.
cmake_minimum_required (VERSION 3.12)

# Enable Hot Reload/Edit and Continue policy for MSVC, if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Define the main project name.
project ("EntityComponentSystem")

# --- Sub-Project Inclusion ---
# Add the subdirectory containing the actual source and build logic.
add_subdirectory ("EntityComponentSystem")
