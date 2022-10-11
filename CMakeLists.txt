cmake_minimum_required(VERSION 3.7)
project(main)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# pon tu directorio donde imgui existe
set(IMGUI_DIR /home/viktor/Downloads/code/cpp/libraries/imgui)
file(GLOB implot_files ${IMGUI_DIR}/implot/*.cpp)

set(TEST_FILES src/test.cpp ${implot_files} ${IMGUI_DIR}/imgui_demo.cpp)
set(SOURCE_FILES src/main.cpp ${implot_files} ${IMGUI_DIR}/imgui_demo.cpp)

include_directories(${IMGUI_DIR})
include_directories(include/)

find_package(fmt)
find_package(ImGui-SFML)
set(LIBS ImGui-SFML::ImGui-SFML fmt::fmt)

add_executable(main ${SOURCE_FILES})
target_link_libraries(main PRIVATE ${LIBS})

add_executable(test ${TEST_FILES})
target_link_libraries(test PRIVATE ${LIBS})