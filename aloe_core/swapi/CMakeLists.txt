

include_directories("${ALOE_SOURCE_DIR}/core/src/common/include")
include_directories("${ALOE_SOURCE_DIR}/core/src/manager/include")
include_directories("${ALOE_SOURCE_DIR}/core/src/manapi/include")
include_directories("${ALOE_SOURCE_DIR}/core/src/node/include")
include_directories("${ALOE_SOURCE_DIR}/core/src/swapi_shared/include")
include_directories("${ALOE_SOURCE_DIR}/core/src/swapi_shared")
include_directories("${ALOE_SOURCE_DIR}/core/include")

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
file(GLOB_RECURSE skeleton_SOURCES "src/skeleton/*.c")

# standalone sources
file(GLOB_RECURSE standalone_SOURCES "src/standalone/*.c")

# aloe core library
add_library(core ${common_SOURCES} ${manager_SOURCES} ${manapi_SOURCES} ${node_SOURCES} ${swapi_shared_SOURCES} ${test_suite_SOURCES})

# swapi library
add_library(swapi ${swapi_static_SOURCES} )

# skeleton library
add_library(skeleton ${swapi_static_SOURCES} ${skeleton_SOURCES})

# standalone library
#add_library(standalone ${standalone_SOURCES})

file(GLOB_RECURSE INCLUDE_FILES "include/*.h")

# install 
install(TARGETS swapi skeleton DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include/aloe)
