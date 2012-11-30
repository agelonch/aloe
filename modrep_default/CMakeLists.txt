if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. This is bad practice.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

cmake_minimum_required (VERSION 2.6)
project (modrep_default)

# The version number.
set (ALOE_VERSION_MAJOR 2)
set (ALOE_VERSION_MINOR 0)

set(CMAKE_BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_C_FLAGS "-Wno-format -std=gnu99 -D_GNU_SOURCE")
set(CMAKE_SHARED_LINKER_FLAGS "-u _run_cycle")

file(GLOB modules *) 

foreach (_module ${modules})
	if (IS_DIRECTORY ${_module})
		if (NOT ${_module} STREQUAL ${CMAKE_BINARY_DIR})
			add_subdirectory(${_module})
		endif()
	endif()
endforeach()

