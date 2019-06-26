project(2019-c1-tp3)

file(GLOB TEST_SOURCES *.cpp)


# Dependencia de Google Test
SET(BUILD_GMOCK OFF)
SET(BUILD_GTEST ON)
add_library(gtest STATIC gtest-1.8.1/gtest-all.cc)
add_library(gtest_main STATIC gtest-1.8.1/gtest_main.cc)

add_executable(correrTests ${TEST_SOURCES} estado/EstadoJuego.cpp estado/EstadoJuego.h)
target_link_libraries(correrTests gtest gtest_main ExtremeExorcism_lib)
