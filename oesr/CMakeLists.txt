if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. This is bad practice.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})


if( CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR )
	cmake_minimum_required (VERSION 2.6)
	project (oesr)

	# The version number.
	set (oesr_VERSION_MAJOR 1)
	set (oesr_VERSION_MINOR 0)
	
	#TODO: If building as standalone, should include rtdal headers here. 
		
else()
	include_directories(${RTDAL_INCLUDE})
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/common/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/manager/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/node/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/oesr_api")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

#remove in the multiprocessing environment
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/node")

#include libconfig directories
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
find_package(LibConfig REQUIRED)
include_directories(${LIBCONFIG_INCLUDE_DIRS})

# oesr sources
file(GLOB_RECURSE common_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.c")
file(GLOB_RECURSE manager_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/manager/*.c")
file(GLOB_RECURSE oesr_man_api_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/oesr_man_api/*.c")
file(GLOB_RECURSE node_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/node/*.c")
file(GLOB_RECURSE oesr_api_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/oesr_api/*.c")
file(GLOB_RECURSE test_suite_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/test_suite/*.c")

# swapi sources
file(GLOB_RECURSE oesrapi_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/oesr_api_static/oesr_static.c")

# skeleton sources
set(skeleton_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/skeleton/skeleton.c)

# params sources
set(params_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/skeleton/params.c)

# mex sources
set(mex_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/skeleton/mex.c)

# standalone sources
file(GLOB_RECURSE standalone_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/standalone/*.c")

# oesr library
add_library(oesr ${oesr_api_SOURCES} ${common_SOURCES} ${manager_SOURCES} ${oesr_man_api_SOURCES} ${node_SOURCES} ${test_suite_SOURCES})
set_target_properties(oesr PROPERTIES LINK_FLAGS "-Wl,-export-dynamic -u _run_main")
target_link_libraries(oesr ${LIBCONFIG_LIBRARIES})

# swapi library
add_library(oesrapi ${oesrapi_SOURCES} ${params_SOURCES})

# skeleton library
add_library(skeleton ${oesrapi_SOURCES} ${skeleton_SOURCES})
set_target_properties(skeleton PROPERTIES COMPILE_FLAGS "-D_COMPILE_ALOE")

# skeleton-legacy library
add_library(skeleton_legacy ${oesrapi_SOURCES} ${skeleton_SOURCES})
set_target_properties(skeleton_legacy PROPERTIES COMPILE_FLAGS "-Wno-implicit-function-declaration -D_COMPILE_ALOE -D_ALOE_OLD_SKELETON")

# standalone library
add_library(standalone ${standalone_SOURCES} ${params_SOURCES})
set_target_properties(standalone PROPERTIES COMPILE_FLAGS "-D_COMPILE_STANDALONE")

# standalone-legacy library
add_library(standalone_legacy ${standalone_SOURCES} ${params_SOURCES})
set_target_properties(standalone_legacy PROPERTIES COMPILE_FLAGS "-Wno-implicit-function-declaration -D_COMPILE_STANDALONE -D_ALOE_OLD_SKELETON")

set(install_mex "")
if(NOT $ENV{OCTAVE_INCLUDE} STREQUAL "") 
	if(NOT $ENV{OCTAVE_LIBS} STREQUAL "") 
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			message(STATUS "Octave for 64-bit not yet tested")
		endif()
		# octave library
		add_library(aloe_octave ${mex_SOURCES} ${params_SOURCES})
		set_target_properties(aloe_octave PROPERTIES COMPILE_FLAGS "-D_COMPILE_MEX -D_FORTIFY_SOURCE=2 -fPIC -I$ENV{OCTAVE_INCLUDE} -I$ENV{OCTAVE_INCLUDE}..  -mieee-fp -g -O0 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security -pthread -fopenmp")

		# octave-legacy library
		add_library(aloe_octave_legacy ${mex_SOURCES} ${params_SOURCES})
		set_target_properties(aloe_octave_legacy PROPERTIES COMPILE_FLAGS "-Wno-implicit-function-declaration -D_ALOE_OLD_SKELETON -D_COMPILE_MEX -D_FORTIFY_SOURCE=2 -fPIC -I$ENV{OCTAVE_INCLUDE} -I$ENV{OCTAVE_INCLUDE}..  -mieee-fp -g -O0 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security -pthread -fopenmp")
		list(APPEND install_mex aloe_octave aloe_octave_legacy)
	endif()
endif()

if(NOT $ENV{MATLAB_ROOT} STREQUAL "") 
		# matlab library
		add_library(aloe_matlab ${mex_SOURCES} ${params_SOURCES})
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set_target_properties(aloe_matlab PROPERTIES COMPILE_FLAGS "-D_COMPILE_MEX -I$ENV{MATLAB_ROOT}/extern/include -I$ENV{MATLAB_ROOT}/simulink/include -DMATLAB_MEX_FILE -ansi -D_GNU_SOURCE -fPIC -fexceptions -fno-omit-frame-pointer -pthread")
		else()
			set_target_properties(aloe_matlab PROPERTIES COMPILE_FLAGS "-D_COMPILE_MEX -I$ENV{MATLAB_ROOT}/extern/include -I$ENV{MATLAB_ROOT}/simulink/include -DMATLAB_MEX_FILE -ansi -D_GNU_SOURCE -fPIC -pthread -m32  -fexceptions -D_FILE_OFFSET_BITS=64 -DMX_COMPAT_32")
		endif()

		# matlab-legacy library
		add_library(aloe_matlab_legacy ${mex_SOURCES} ${params_SOURCES})
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set_target_properties(aloe_matlab_legacy PROPERTIES COMPILE_FLAGS "-D_COMPILE_MEX -I$ENV{MATLAB_ROOT}/extern/include -I$ENV{MATLAB_ROOT}/simulink/include -DMATLAB_MEX_FILE -ansi -D_GNU_SOURCE -fPIC -fexceptions -fno-omit-frame-pointer -pthread")
		else()
			set_target_properties(aloe_matlab_legacy PROPERTIES COMPILE_FLAGS "-Wno-implicit -D_COMPILE_MEX -I$ENV{MATLAB_ROOT}/extern/include -I$ENV{MATLAB_ROOT}/simulink/include -DMATLAB_MEX_FILE -ansi -D_GNU_SOURCE -fPIC -pthread -m32  -fexceptions -D_FILE_OFFSET_BITS=64 -DMX_COMPAT_32")
		endif()
		list(APPEND install_mex aloe_matlab aloe_matlab_legacy)
endif()





file(GLOB_RECURSE INCLUDE_FILES "include/*.h")

# install 
install(TARGETS oesrapi skeleton skeleton_legacy standalone standalone_legacy ${install_mex} DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include)
