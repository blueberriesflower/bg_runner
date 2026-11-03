#!/bin/bash
set -e

git pull || echo "Не удалось обновить репозиторий. Сборка локальной версии"

mkdir -p build
cd build

cmake ..
make