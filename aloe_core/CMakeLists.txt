if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. This is bad practice.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

cmake_minimum_required (VERSION 2.6)
project (ALOE)

# The version number.
set (ALOE_VERSION_MAJOR 2)
set (ALOE_VERSION_MINOR 0)

set(CMAKE_BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories("${PROJECT_SOURCE_DIR}/swapi/include/")
include_directories("${PROJECT_SOURCE_DIR}/swapi/include/aloe/")

set(CMAKE_C_FLAGS "-Wno-format -std=gnu99 -D_GNU_SOURCE -fPIC")

include_directories("${ALOE_SOURCE_DIR}/hwapi_lnx/include")
include_directories("${ALOE_SOURCE_DIR}/swapi/include")

add_subdirectory(swapi)
add_subdirectory(hwapi_lnx)
