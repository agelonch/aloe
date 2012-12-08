if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. This is bad practice.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})


if( CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR )
	cmake_minimum_required (VERSION 2.6)
	project (oecore)

	# The version number.
	set (OECORE_VERSION_MAJOR 1)
	set (OECORE_VERSION_MINOR 0)
	
	#TODO: If building as standalone, should include hwapi headers here. 
	
	
else()
	include_directories(${ALCORE_INCLUDE})
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/common/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/manager/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/node/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/swapi_shared")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

#remove in the multiprocessing environment
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/node")


# core sources
file(GLOB_RECURSE common_SOURCES "src/common/*.c")
file(GLOB_RECURSE manager_SOURCES "src/manager/*.c")
file(GLOB_RECURSE manapi_SOURCES "src/manapi/*.c")
file(GLOB_RECURSE node_SOURCES "src/node/*.c")
file(GLOB_RECURSE swapi_shared_SOURCES "src/swapi_shared/*.c")
file(GLOB_RECURSE test_suite_SOURCES "src/test_suite/*.c")

# swapi sources
file(GLOB_RECURSE swapi_static_SOURCES "src/swapi_static/*.c")

# skeleton sources
set(skeleton_SOURCES src/skeleton/skeleton.c)

# params sources
set(params_SOURCES src/skeleton/params.c)

# mex sources
set(mex_SOURCES src/skeleton/mex.c)

# standalone sources
file(GLOB_RECURSE standalone_SOURCES "src/standalone/*.c")

# aloe core library
add_library(oecore ${swapi_shared_SOURCES} ${common_SOURCES} ${manager_SOURCES} ${manapi_SOURCES} ${node_SOURCES} ${test_suite_SOURCES})
set_target_properties(oecore PROPERTIES LINK_FLAGS "-Wl,-export-dynamic -u _run_main")

# swapi library
add_library(swapi ${swapi_static_SOURCES} ${params_SOURCES})

# skeleton library
add_library(skeleton ${swapi_static_SOURCES} ${skeleton_SOURCES})
set_target_properties(skeleton PROPERTIES COMPILE_FLAGS "-D_COMPILE_ALOE")

# skeleton-legacy library
add_library(skeleton_legacy ${swapi_static_SOURCES} ${skeleton_SOURCES})
set_target_properties(skeleton_legacy PROPERTIES COMPILE_FLAGS "-Wno-implicit -D_COMPILE_ALOE -D_ALOE_OLD_SKELETON")

# standalone library
add_library(standalone ${standalone_SOURCES} ${params_SOURCES})
set_target_properties(standalone PROPERTIES COMPILE_FLAGS "-D_COMPILE_STANDALONE")

# standalone-legacy library
add_library(standalone_legacy ${standalone_SOURCES} ${params_SOURCES})
set_target_properties(standalone_legacy PROPERTIES COMPILE_FLAGS "-Wno-implicit -D_COMPILE_STANDALONE -D_ALOE_OLD_SKELETON")

set(install_mex "")
if(NOT $ENV{OCTAVE_INCLUDE} STREQUAL "") 
	if(NOT $ENV{OCTAVE_LIBS} STREQUAL "") 
		# octave library
		add_library(aloe_octave ${mex_SOURCES} ${params_SOURCES})
		set_target_properties(aloe_octave PROPERTIES COMPILE_FLAGS "-D_COMPILE_MEX -D_FORTIFY_SOURCE=2 -fPIC -I$ENV{OCTAVE_INCLUDE} -I$ENV{OCTAVE_INCLUDE}..  -mieee-fp -g -O0 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security -pthread -fopenmp")

		# octave-legacy library
		add_library(aloe_octave_legacy ${mex_SOURCES} ${params_SOURCES})
		set_target_properties(aloe_octave_legacy PROPERTIES COMPILE_FLAGS "-Wno-implicit -D_ALOE_OLD_SKELETON -D_COMPILE_MEX -D_FORTIFY_SOURCE=2 -fPIC -I$ENV{OCTAVE_INCLUDE} -I$ENV{OCTAVE_INCLUDE}..  -mieee-fp -g -O0 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security -pthread -fopenmp")
		list(APPEND install_mex aloe_octave aloe_octave_legacy)
	endif()
endif()

if(NOT $ENV{MATLAB_ROOT} STREQUAL "") 
		# matlab library
		add_library(aloe_matlab ${mex_SOURCES} ${params_SOURCES})
		set_target_properties(aloe_matlab PROPERTIES COMPILE_FLAGS "-D_COMPILE_MEX -I$ENV{MATLAB_ROOT}/extern/include -I$ENV{MATLAB_ROOT}/simulink/include -DMATLAB_MEX_FILE -ansi -D_GNU_SOURCE -fPIC -pthread -m32  -fexceptions -D_FILE_OFFSET_BITS=64 -DMX_COMPAT_32")

		# matlab-legacy library
		add_library(aloe_matlab_legacy ${mex_SOURCES} ${params_SOURCES})
		set_target_properties(aloe_matlab_legacy PROPERTIES COMPILE_FLAGS "-Wno-implicit -D_COMPILE_MEX -I$ENV{MATLAB_ROOT}/extern/include -I$ENV{MATLAB_ROOT}/simulink/include -DMATLAB_MEX_FILE -ansi -D_GNU_SOURCE -fPIC -pthread -m32  -fexceptions -D_FILE_OFFSET_BITS=64 -DMX_COMPAT_32")
		list(APPEND install_mex aloe_matlab aloe_matlab_legacy)
endif()





file(GLOB_RECURSE INCLUDE_FILES "include/*.h")

# install 
install(TARGETS swapi skeleton skeleton_legacy standalone standalone_legacy ${install_mex} DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include/aloe)
