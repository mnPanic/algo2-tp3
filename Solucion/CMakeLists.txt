# Reference: https://arne-mertz.de/2018/06/cmake-project-structure/

cmake_minimum_required(VERSION 3.5)
project(2019-c1-tp3)

option(WITH_GUI "Preparar compilación con interfaz gráfica (necesita SDL instalado)" OFF)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
# Algunos flags para pasar al compilador (gnu++11 en vez de c++11 para que sea cross-plat)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=gnu++11 -ggdb3 -g")

add_subdirectory("src")
add_subdirectory("tests")

if (WITH_GUI)
    set_target_properties(ExtremeExorcism PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif (WITH_GUI)

set_target_properties(correrTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR}/bin)
