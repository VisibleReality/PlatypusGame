cmake_minimum_required(VERSION 3.26)
project(PlatypusGame)

set(CMAKE_CXX_STANDARD 17)

add_link_options(-static-libgcc -static-libstdc++)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
	add_compile_options(-O0)
else ()
	add_compile_options(-O3)
endif ()

add_executable(PlatypusGame main.cpp
		PlatypusGame.cpp
		RoundRobinTournament.cpp
		TournamentRunner.cpp
		PartialRoundRobinTournament.cpp
		EloRandomTournament.cpp
		GlickoRandomTournament.cpp
		include/glicko2/rating.cpp
		SingleEliminationTournament.cpp
)
