cmake_minimum_required(VERSION 3.26)
project(PlatypusGame)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Og)

add_executable(PlatypusGame main.cpp
		PlatypusGame.cpp
		PlatypusGame.h
		Result.cpp
		Result.h)
