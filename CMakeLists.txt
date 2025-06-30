cmake_minimum_required(VERSION 3.30)
project(GameSimulator)

set(CMAKE_CXX_STANDARD 11)

add_executable(GameSimulator
               main.cpp
               Geometry.cpp
               Controller.cpp
               Warehouse.cpp
               Trackbase.cpp
               Truck.cpp
               Model.cpp
               )
