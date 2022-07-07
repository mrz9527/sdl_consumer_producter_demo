cmake_minimum_required(VERSION 3.5)

project(consumer_producter_demo)

set(CMAKE_CXX_STANDARD 11)

set(SDL_HOME ${PROJECT_BINARY_DIR}/sdl)
include_directories(${SDL_HOME}/include/SDL2)
link_directories(${SDL_HOME}/lib)

add_executable(sdl_consumer_producter_demo consumer_producter_demo.cpp)
target_link_libraries(sdl_consumer_producter_demo SDL2)
