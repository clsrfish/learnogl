#!/bin/bash

if [ ! -d build ]; then
    echo "-- Build directory not exists"
fi

cmake -S . -B build --log-level=VERBOSE -Wdev

if [ $(uname) == "Linux" ]; then
    cpu_core=$(nproc)
elif [ $(uname) == "Darwin" ]; then
    cpu_core=$(sysctl -n hw.physicalcpu)
else
    echo "Unsupported platform"
    exit 1
fi
echo "-- CPU core: $cpu_core"

make -d -j${cpu_core} -C build

echo "-- Launching main"
./build/main
