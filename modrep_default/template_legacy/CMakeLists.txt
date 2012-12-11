# This configuration is for the aloe1.5 skeleton-legacy version

# set-up the program libraries here
set(LIBRARIES m rt)

file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE TEST_SOURCES "test/*.c")

get_filename_component(module ${CMAKE_CURRENT_SOURCE_DIR} NAME) 

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)

# aloe module
add_library(${module}-aloe SHARED ${SOURCES})
set_target_properties(${module}-aloe PROPERTIES OUTPUT_NAME ${module})
set_target_properties(${module}-aloe PROPERTIES COMPILE_FLAGS "-D_COMPILE_ALOE -D_ALOE_OLD_SKELETON")
target_link_libraries(${module}-aloe oesrapi skeleton_legacy ${LIBRARIES})
install(TARGETS ${module}-aloe DESTINATION lib/${MODULE_REPOS_NAME}/)


# standalone program for testing
add_executable(${module}-bin ${SOURCES} ${TEST_SOURCES})
set_target_properties(${module}-bin PROPERTIES OUTPUT_NAME ${module})
set_target_properties(${module}-bin PROPERTIES COMPILE_FLAGS "-D_COMPILE_STANDALONE -D_ALOE_OLD_SKELETON")
target_link_libraries(${module}-bin standalone_legacy ${LIBRARIES})
install(TARGETS ${module}-bin DESTINATION bin)


# octave mex file 
set(install_mex "")
if(NOT $ENV{OCTAVE_INCLUDE} STREQUAL "") 
	if(NOT $ENV{OCTAVE_LIBS} STREQUAL "") 
	
	add_library(${module}-oct SHARED ${SOURCES})
	set_target_properties(${module}-oct PROPERTIES OUTPUT_NAME ${module})
	set_target_properties(${module}-oct PROPERTIES PREFIX "am_")
	set_target_properties(${module}-oct PROPERTIES SUFFIX .mex)
	
	set_target_properties(${module}-oct PROPERTIES COMPILE_FLAGS "-I$ENV{OCTAVE_INCLUDE} -D_COMPILE_MEX -D_ALOE_OLD_SKELETON -Wl,-Bsymbolic -L$ENV{OCTAVE_LIBS} -loctinterp -loctave -lcruft -Wl,-Bsymbolic-functions -Wl,-z,relro")
	target_link_libraries(${module}-oct aloe_octave_legacy ${LIBRARIES})
	install(TARGETS ${module}-oct DESTINATION mex)
	
	endif()
endif()

if(NOT $ENV{MATLAB_ROOT} STREQUAL "") 
	add_library(${module}-mat SHARED ${SOURCES})
	set_target_properties(${module}-mat PROPERTIES OUTPUT_NAME ${module})
	set_target_properties(${module}-mat PROPERTIES PREFIX "am_")
	
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set_target_properties(${module}-mat PROPERTIES SUFFIX .mexa64)
		set_target_properties(${module}-mat PROPERTIES COMPILE_FLAGS "-D_ALOE_OLD_SKELETON -I$ENV{MATLAB_ROOT} -O -pthread -shared -Wl,--version-script,$ENV{MATLAB_ROOT}/extern/lib/glnxa64/mexFunction.map -Wl,--no-undefined -Wl,-rpath-link,$ENV{MATLAB_ROOT}/bin/glnxa64 -L$ENV{MATLAB_ROOT}/bin/glnxa64 -lmx -lmex -lmat -lm -lstdc++")
	else()
		set_target_properties(${module}-mat PROPERTIES SUFFIX .mexglx)
	set_target_properties(${module}-mat PROPERTIES COMPILE_FLAGS "-D_ALOE_OLD_SKELETON -I$ENV{MATLAB_ROOT} -O -pthread -shared -m32 -Wl,--version-script,$ENV{MATLAB_ROOT}/extern/lib/glnx86/mexFunction.map -Wl,--no-undefined -Wl,-rpath-link,$ENV{MATLAB_ROOT}/bin/glnx86 -L$ENV{MATLAB_ROOT}/bin/glnx86 -lmx -lmex -lmat -lm -lstdc++")
	endif()
	
	target_link_libraries(${module}-mat aloe_matlab_legacy ${LIBRARIES})
	install(TARGETS ${module}-mat DESTINATION mex)
	
endif()











