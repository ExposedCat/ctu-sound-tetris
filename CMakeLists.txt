cmake_minimum_required(VERSION 3.10)
project(main)
include_directories(dependencies/include)
link_directories(dependencies)
add_executable(main src/controller.cpp src/background-sound.cpp src/brick-sound.cpp src/brick.cpp src/utils.cpp src/main.cpp)
target_link_libraries(main iimav)
