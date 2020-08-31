cmake_minimum_required(VERSION 3.16)
project(LuaLink)

set(CMAKE_CXX_STANDARD 20)

add_executable(LuaLink main.cpp parameters.cpp parameters.h utils.cpp utils.h)