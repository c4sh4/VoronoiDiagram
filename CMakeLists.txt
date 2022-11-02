cmake_minimum_required(VERSION 3.23)
project(VoronoiDiagram)

set(CMAKE_CXX_STANDARD 17)

add_executable(VoronoiDiagram main.cpp Vec.cpp Vec.h halfPlane.cpp halfPlane.h interAlg.cpp interAlg.h)
