@echo off
mkdir build
cd build
cmake ..
cmake --build .
cmake --build . --target install
pause
