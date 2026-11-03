cmake_minimum_required(VERSION 3.10)
project(BackgroundRunner LANGUAGES C)

set(CMAKE_C_STANDARD 11)

# Статическая библиотека
add_library(bg_runner STATIC src/bg_runner.h)
target_include_directories(bg_runner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Выбираем реализацию в зависимости от платформы
if(WIN32)
    target_sources(bg_runner PRIVATE src/bg_runner_windows.c)
else()
    target_sources(bg_runner PRIVATE src/bg_runner_linux.c)
endif()

# Тестовая утилита
add_executable(test_runner src/test.c)
target_link_libraries(test_runner PRIVATE bg_runner)