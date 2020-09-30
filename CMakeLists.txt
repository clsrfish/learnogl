cmake_minimum_required(VERSION 3.0.0)
project(demo VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file (
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/config.h
)

include_directories(
    /usr/local/include
    ${PROJECT_BINARY_DIR}
)
link_directories(
    /usr/local/lib
    ${PROJECT_BINARY_DIR}
)

# add_library(ur_lib
#     SHARED
#     IMPORTED
# )
# set_target_properties(ur_lib 
#     PROPERTIES 
#     IMPORTED_LOCATION 
#     ${PROJECT_SOURCE_DIR}/path/to/ur_lib)

add_subdirectory(src)

add_executable(main main.cpp)
target_include_directories(
    main PUBLIC
    ${PROJECT_BINARY_DIR}
)
target_link_libraries(main src)

# Test case 
include(CTest)
enable_testing()

# build package installer start
include (InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE  "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
# build package installer end
