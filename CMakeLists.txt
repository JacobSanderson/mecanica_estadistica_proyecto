cmake_minimum_required(VERSION 3.7)
project(main)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILES src/main.cpp /home/viktor/Downloads/code/cpp/libraries/imgui/imgui_demo.cpp)

include_directories(/usr/local/include/)
include_directories(/home/viktor/Downloads/code/cpp/libraries/imgui)

include_directories(include/)

find_package(fmt)
find_package(ImGui-SFML)
# find_package(SFML 2.5 COMPONENTS graphics window system audio)

# set(SFML_STATIC_LIBRARIES TRUE)
#  sfml-audio sfml-graphics sfml-window sfml-system 
set(LIBS ImGui-SFML::ImGui-SFML fmt::fmt)

add_executable(main ${SOURCE_FILES})
target_link_libraries(main PRIVATE ${LIBS})
