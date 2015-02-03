cmake_minimum_required(VERSION 2.8.4)
project(CPP_Practice)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCE_FILES
    Evaluators/ThreadedEvaluator.cpp
    Evaluators/SimpleEvaluator.cpp
    Boards/VectorBoard.cpp
    Boards/Board.cpp
    Boards/Board.h
    main.cpp)

add_executable(CPP_Practice ${SOURCE_FILES})