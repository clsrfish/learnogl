cmake_minimum_required(VERSION 3.0.0)
project(demo VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file (
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/config.h
)

IF (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    ADD_DEFINITIONS(-DWindows)
ELSE (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
    ADD_DEFINITIONS(-DLinux)
ENDIF ()

include_directories(
    /usr/local/include
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/libs/glad/include 
    ${PROJECT_SOURCE_DIR}/libs/glfw/include
    ${PROJECT_SOURCE_DIR}/libs/stb_image/include
)
link_directories(
    /usr/local/lib
    ${PROJECT_BINARY_DIR}
)

# GLFW lib
add_library(glfw 
    SHARED 
    IMPORTED)
set_target_properties(glfw 
    PROPERTIES 
    IMPORTED_LOCATION 
    ${PROJECT_SOURCE_DIR}/libs/glfw/lib/libglfw.3.dylib)

set(LINK_LIBS glad glfw stb_image)

add_subdirectory(src)
add_subdirectory(libs)

add_executable(main main.cpp)
target_include_directories(
    main PUBLIC
    ${PROJECT_BINARY_DIR}
)
target_link_libraries(main src ${LINK_LIBS})

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
