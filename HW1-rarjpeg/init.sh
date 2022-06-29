#!/bin/sh

if [ -e build ]; then
    rm -r build
fi

cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
