
include_directories("${ALOE_SOURCE_DIR}/src")

file(GLOB_RECURSE hwapi_SOURCES "${ALOE_SOURCE_DIR}/hwapi_lnx/src/*.c")

# runcf executable
add_executable(runcf ${hwapi_SOURCES})
target_link_libraries (runcf "-Wl,--whole-archive" core)
target_link_libraries (runcf "-Wl,--no-whole-archive" rt dl config)



# install runcf
install(TARGETS runcf DESTINATION bin)
