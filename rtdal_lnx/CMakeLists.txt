if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. This is bad practice.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})


if( CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR )
	cmake_minimum_required (VERSION 2.6)
	project (alcore_lnx)

	# The version number.
	set (ALCORE_LNX_VERSION_MAJOR 1)
	set (ALCORE_LNX_VERSION_MINOR 0)
endif()


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE rtdal_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# runcf executable
add_executable(runcf ${rtdal_SOURCES})
target_link_libraries (runcf "-Wl,--whole-archive" oesr)
target_link_libraries (runcf "-Wl,--no-whole-archive" rt dl config)



# install runcf
install(TARGETS runcf DESTINATION bin)
