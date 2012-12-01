# This configuration is for using plain module development (without skeleton)

file(GLOB_RECURSE SOURCES "src/*.c")

get_filename_component(module ${CMAKE_CURRENT_SOURCE_DIR} NAME) 
add_library(${module} SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(${module} swapi)
install(TARGETS ${module} DESTINATION lib/${MODULE_REPOS_NAME}/)
