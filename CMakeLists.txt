if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. This is bad practice.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

cmake_minimum_required (VERSION 2.6)
project (ALOE++)

# The version number.
set (ALOE_VERSION_MAJOR 0)
set (ALOE_VERSION_MINOR 2)

set(MODULE_REPOS_NAME "default")

set(CMAKE_C_FLAGS "-Wall -Winline -Wno-format -std=gnu99 -D_GNU_SOURCE -fPIC")
set(CMAKE_BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(RTDAL_INCLUDE ${CMAKE_SOURCE_DIR}/rtdal_lnx/include)
set(OESR_INCLUDE ${CMAKE_SOURCE_DIR}/oesr/include)

add_subdirectory(oesr)
add_subdirectory(rtdal_lnx)
add_subdirectory(modrep_default)
add_subdirectory(modrep_ofdm)
