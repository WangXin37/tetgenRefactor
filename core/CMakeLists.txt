file(GLOB_RECURSE SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.cxx")
include_directories(${THIRDPARTY_DIR})
include_directories(${THIRDPARTY_DIR}/predicates)
add_library(core ${SOURCE_FILES} ${THIRDPARTY_DIR}/predicates/predicates.c)
target_include_directories(core PRIVATE 
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/core 
    ${THIRDPARTY_DIR}/predicates
    ${THIRDPARTY_DIR})