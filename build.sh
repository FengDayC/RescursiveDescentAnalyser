#!/bin/sh
mkdir build
cd build
cmake ..
cmake --build .
cmake --build . --target install
