cmake_minimum_required(VERSION 3.0.0)
project(learnogl VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
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
    ${PROJECT_SOURCE_DIR}/libs/assimp/include 
    ${PROJECT_SOURCE_DIR}/libs/freetype/include 
    ${PROJECT_SOURCE_DIR}/libs/glad/include 
    ${PROJECT_SOURCE_DIR}/libs/glfw/include
    ${PROJECT_SOURCE_DIR}/libs/glm/include
    ${PROJECT_SOURCE_DIR}/libs/irrklang/include
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

# assimp lib
add_library(assimp 
    SHARED 
    IMPORTED)
set_target_properties(assimp 
    PROPERTIES 
    IMPORTED_LOCATION 
    ${PROJECT_SOURCE_DIR}/libs/assimp/lib/libassimp.5.dylib)
# irrKlang
add_library(irrklang 
    SHARED 
    IMPORTED)
set_target_properties(irrklang 
    PROPERTIES 
    IMPORTED_LOCATION 
    ${PROJECT_SOURCE_DIR}/libs/irrklang/lib/libirrklang.dylib)
# freetype
add_library(freetype 
    SHARED 
    IMPORTED)
set_target_properties(freetype 
    PROPERTIES 
    IMPORTED_LOCATION 
    ${PROJECT_SOURCE_DIR}/libs/freetype/lib/libfreetype.6.16.0.dylib)

set(LINK_LIBS assimp freetype glad glfw irrklang stb_image)

add_subdirectory(src)
add_subdirectory(libs)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_BINARY_DIR}
)
target_link_libraries(${PROJECT_NAME} src ${LINK_LIBS})

if(APPLE)
    message("Adding add_custom_command")
    add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND echo "Changing irrklang reference path"
    COMMAND install_name_tool -add_rpath ${PROJECT_SOURCE_DIR}/libs/irrklang/lib 
    # -change /usr/local/lib/libikpFLAC.dylib @rpath/ikpFLAC.dylib 
    # -change /usr/local/lib/libikpMP3.dylib @rpath/ikpMP3.dylib 
    -change /usr/local/lib/libirrklang.dylib @rpath/libirrklang.dylib ${PROJECT_NAME}
    COMMAND ln -sf ${PROJECT_SOURCE_DIR}/libs/irrklang/lib/ikpFLAC.dylib ${PROJECT_SOURCE_DIR}/ikpFLAC.dylib
    COMMAND ln -sf ${PROJECT_SOURCE_DIR}/libs/irrklang/lib/ikpMP3.dylib ${PROJECT_SOURCE_DIR}/ikpMP3.dylib
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
endif(APPLE)

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
