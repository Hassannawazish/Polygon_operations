cmake_minimum_required(VERSION 3.20)
project(PolygonUnionExample)

set(CMAKE_CXX_STANDARD 14)  # Use C++14 or higher

add_executable(PolygonUnionExample main.cpp Point.cpp LineDrawing.cpp PolygonValidation.cpp Intersection.cpp Union.cpp Difference.cpp GridVisualization.cpp)
