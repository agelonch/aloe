if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. This is bad practice.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

cmake_minimum_required (VERSION 2.6)
project (ALOE_PARENT)

# The version number.
set (ALOE_PARENT_VERSION_MAJOR 2)
set (ALOE_PARENT_VERSION_MINOR 0)

include_directories("${PROJECT_SOURCE_DIR}/aloe_core/swapi/include/")
include_directories("${PROJECT_SOURCE_DIR}/aloe_core/swapi/include/aloe/")

set(MODULE_REPOS_NAME "default")

set(CMAKE_C_FLAGS "-Wno-format -std=gnu99 -D_GNU_SOURCE -fPIC")
set(CMAKE_BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories("${ALOE_SOURCE_DIR}/hwapi_lnx/include")
include_directories("${ALOE_SOURCE_DIR}/swapi/include")


add_subdirectory(alcore_lnx)
add_subdirectory(oecore)
add_subdirectory(modrep_default)
add_subdirectory(modrep_ofdm)
