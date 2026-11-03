@echo off

git pull

if not exist build mkdir build
cd build

cmake .. -G "MinGW Makefiles"

mingw32-make

echo Сборка завершена!
hello_world.exe
pause