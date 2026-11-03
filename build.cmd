@echo off

git pull

if not exist build mkdir build
cd build

cmake .. -G "MinGW Makefiles"

mingw32-make

echo Build Complete!
pause