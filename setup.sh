#!/bin/bash
cwd=$(dirname $0)

if [ ! -d .tmp ]; then
    mkdir .tmp
fi

cd .tmp

# glfw
if [ ! -e glfw.zip ]; then
    echo "Downloading glfw"
    curl -L https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.bin.MACOS.zip -o glfw.zip
    unzip -o glfw.zip
    cp -r glfw-3.3.2.bin.MACOS/include ${cwd}/libs/glfw/
    cp glfw-3.3.2.bin.MACOS/lib-macos/libglfw.3.dylib ${cwd}/libs/glfw/lib/
fi

# glad
if [ ! -e glad.zip ]; then
    echo "Downloading glad"
    curl -L 'https://glad.dav1d.de/generated/tmphDUh2qglad/glad.zip' -o glad.zip
    unzip -o glad.zip -d glad
    cp -r glad/include glad/src ${cwd}/libs/glad/
fi

# glm
if [ ! -e glm.zip ]; then
    echo "Downloading glm"
    curl -L 'https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip' -o glm.zip
    unzip -o glm.zip
    cp -r glm/glm/* ${cwd}/libs/glm/include/
fi

# assimp
if [ ! -e assimp.zip ]; then
    echo "Downloading assimp"
    curl -L 'https://github.com/assimp/assimp/archive/v5.0.1.zip' -o assimp.zip
    unzip -o assimp.zip
    cd assimp-5.0.1
    if [ ! -d build ]; then
        mkdir build
    fi
    cd build
    cmake .. && make
    cd ../
    cp -r include build/include ${cwd}/libs/assimp/include
    cp build/code/libassimp.5.0.0.dylib ${cwd}/libs/assimp/lib/libassimp.5.dylib
    cd ../
fi

# freetype
if [ ! -e freetype.tar.gz ]; then
    echo "Downloading freetype"
    curl -L 'https://download.savannah.gnu.org/releases/freetype/freetype-2.10.0.tar.gz' -o freetype.tar.gz
    tar -zxvf freetype.tar.gz
    cd freetype-2.10.0 && mkdir build && cd build
    cmake -D BUILD_SHARED_LIBS:BOOL=true .. && make
    cd ../
    cp -r include ${cwd}/lib/freetype/
    cp build/libfreetype.6.16.0.dylib ${cwd}/lib/freetype/lib/
    cd ../
fi
