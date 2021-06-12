cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)

project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Threads)
include_directories(${SDL2_INCLUDE_DIRS} include)

LIST(APPEND SOURCE_CPPS
    src/game.cpp
    src/controller.cpp 
    src/renderer.cpp 
    src/game_object.cpp 
    src/paddle.cpp 
    src/ball.cpp 
    src/collision_engine.cpp
    src/score.cpp
)

add_executable(PongGame src/main.cpp ${SOURCE_CPPS})
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(PongGame ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
