project(2019-c1-tp3)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

file(GLOB SOURCE_FILES modelo/*.cpp modelo/*.h 
    modelo/localizadores/*.cpp modelo/localizadores/*.h modulos_basicos/*.h
    modulos_basicos/*.hpp modulos_basicos/*.cpp)

if (WITH_GUI)
  file(GLOB GUI_SOURCE_FILES gui/*.h gui/*.hpp gui/*.cpp)
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
  string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
  add_library(ExtremeExorcism_gui_lib STATIC ${GUI_SOURCE_FILES})
  target_link_libraries(ExtremeExorcism_gui_lib ExtremeExorcism_lib)
endif(WITH_GUI)

add_library(ExtremeExorcism_lib STATIC ${SOURCE_FILES})
target_include_directories(ExtremeExorcism_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (WITH_GUI)
  add_executable(ExtremeExorcism main.cpp)
  target_link_libraries(ExtremeExorcism ${SDL2_LIBRARIES}
      ${SDL2_IMAGE_LIBRARIES} ExtremeExorcism_lib ExtremeExorcism_gui_lib)
endif(WITH_GUI)
