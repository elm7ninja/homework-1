cmake_minimum_required(VERSION 4.0.3)
project(build NONE)

# Enable C++23 and module support
# https://github.com/Kitware/CMake/blob/v4.0.3/Help/dev/experimental.rst
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")  # Watch this GUID, it may change with future CMake versions

if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose build type: " FORCE)
endif()
message(STATUS "Configuring for a build type of:  ${CMAKE_BUILD_TYPE}\n--")

if( NOT ProjectName )
  set(ProjectName project CACHE STRING "Project name: " FORCE)
endif()

# Expected cmake to be launched from a script (e.g., Build.sh) with something similar to:
#   cmake -S "${0%/*}/cmake"  -B "${PWD}/.build/clang"
# where the cmake subdirectory is in the in the same directory as the script but the script is not in the current directory
# and .build/** is found (or created) in the current directory
set(Project_Root ${CMAKE_BINARY_DIR}/../..)
cmake_path(ABSOLUTE_PATH Project_Root NORMALIZE)


##############################################################
# Workaround File
# Vendors are releasing version of their C++ compilers and libraries with ever more C++23 compliant features, but
# they're not fully compliant yet.  And Linux distributors are slow to make these new versions available.  As a result,
# we see in practice various degrees of compliance which we can compensate for by providing the missing pieces in a
# header file that is added to each translation unit.  We search for such a header file first in the current
# directory, then in the same directory as the build script.
# This search order allows such a (potentially updated) header file to be provided with each project.
##############################################################

# Look for the compliance workaround file in a prioritized path list
# Pass along to subdirectory builds
set(COMPLIANCE_HEADER_FILE "Compliance_Workarounds.hpp")
find_path(COMPLIANCE_HEADER_PATH
  NAMES "${COMPLIANCE_HEADER_FILE}"
  PATHS "${Project_Root}"                    # First look in the source code directory
        "${CMAKE_SOURCE_DIR}/.."             # if not there, look next to the Build.sh script
        NO_CACHE
        NO_DEFAULT_PATH)

if(COMPLIANCE_HEADER_PATH)
  # I want the COMPLIANCE_HEADER_PATH to be an absolute path (for downstream processing), but I
  # want to display it as a relative path
  cmake_path(RELATIVE_PATH  COMPLIANCE_HEADER_PATH BASE_DIRECTORY "${Project_Root}" OUTPUT_VARIABLE rel_path)
  string(REGEX REPLACE "^\.$" "./" rel_path "${rel_path}")
  message(STATUS "Using compliance header: \"${rel_path}${COMPLIANCE_HEADER_FILE}\"   (relative to ${Project_Root})\n--")
else()
  message(STATUS "Not using a compliance workaround header\n--")
endif()


#  Generates a compile_commands.json file containing the exact compiler calls for all translation units of
#  the project in machine-readable form.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


##############################################################
# Locate non-module source files, excluding CMAKE_BINARY_DIR
##############################################################
function(exclude_hidden_directories list_var)
  set(_filtered "")
  foreach(item IN LISTS ${list_var})
    cmake_path(NORMAL_PATH item)
    string(FIND "${item}" "${Project_Root}." match_pos)
    if(NOT match_pos EQUAL 0)
      list(APPEND _filtered "${item}")
    endif()
  endforeach()

  # Push the updated list back to the caller
  set( ${list_var} "${_filtered}" PARENT_SCOPE)
endfunction()


# Glob non-module sources (e.g. main.cpp, helpers)
# Globs are recalculated only during the config phase (cmake -S xx -B yy), not build phase (cmake --build zz)
file(GLOB_RECURSE SRC_HEADERS                             # IDE support
  "${Project_Root}/*.hpp"
  "${Project_Root}/*.hxx"
  "${Project_Root}/*.h"
  "${Project_Root}/*.inc"
)
exclude_hidden_directories(SRC_HEADERS)

file(GLOB_RECURSE SRC_SOURCES
  "${Project_Root}/*.cpp"
  "${Project_Root}/*.cxx"
  "${Project_Root}/*.cc"
)
exclude_hidden_directories(SRC_SOURCES)





##############################################################
# Locate module interface (.cppm, .ixx)
##############################################################
# Globs are recalculated only during the config phase (cmake -S xx -B yy), not build phase (cmake --build zz)
file(GLOB_RECURSE MODULE_INTERFACES
  "${Project_Root}/*.cppm"
  "${Project_Root}/*.ixx"
)
exclude_hidden_directories(MODULE_INTERFACES)


##############################################################
# If there are source files, define the executable
# target and its dependencies
##############################################################
# Create executable only if non-module sources exist
# At least one file is required (something containing int main(), for example)
if(SRC_SOURCES)
  # Display files comprising the build, weather or not they are out of date
  message(STATUS "Building with: (relative to \"${Project_Root}\")")
  foreach(file IN LISTS SRC_SOURCES MODULE_INTERFACES)
    file(RELATIVE_PATH rel_file "${Project_Root}" "${file}")
    message(STATUS "     ${rel_file}")
  endforeach()
  message(STATUS)
  message(STATUS)
  message(STATUS)

  # Expect cmake to be called with either gcc or clang as the build directory, for example:
  #  cmake -S ./cmake  -B .build/clang
  #  cmake -S ./cmake  -B .build/gcc
  cmake_path(GET CMAKE_BINARY_DIR FILENAME tool_chain)
  include(${tool_chain}.cmake)

else()
  message(STATUS "*** Nothing to build, no (non-module) source file found in ${Project_Root}/src/**\n       At least one source file required")
endif()
