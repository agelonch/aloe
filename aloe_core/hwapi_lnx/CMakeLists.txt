
include_directories("${ALOE_SOURCE_DIR}/src")
include_directories("${ALOE_SOURCE_DIR}/core/include")

file(GLOB_RECURSE hwapi_SOURCES "${ALOE_SOURCE_DIR}/hwapi_lnx/src/*.c")

# runcf executable
add_executable(runcf ${hwapi_SOURCES})
target_link_libraries (runcf rt dl core config)

# install runcf
install(TARGETS runcf DESTINATION bin)
