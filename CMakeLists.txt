cmake_minimum_required(VERSION 3.26)
project(PlatypusGame)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-O0)

add_executable(PlatypusGame main.cpp
		PlatypusGame.cpp
		PlatypusGame.h
		Tournament.h
		RoundRobinTournament.cpp
		RoundRobinTournament.h)
