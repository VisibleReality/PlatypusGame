#include <iostream>
#include <vector>
#include "PlatypusGame.h"
#include "Tournament.h"
#include "RoundRobinTournament.h"

int main()
{
	std::vector<unsigned long> players;

	players.push_back(1);
	players.push_back(2);
	players.push_back(3);
	players.push_back(4);

	RoundRobinTournament tournament = RoundRobinTournament(players);

	auto games = tournament.getNextGames(9);
	std::cout << "set1" << std::endl;
	for (const auto& game : games)
	{
		std::cout << game->at(0) << " " << game->at(1) << std::endl;
	}

	std::cout << tournament.tournamentDone() << std::endl;

	games = tournament.getNextGames(9);
	std::cout << "set2" << std::endl;
	for (const auto& game : games)
	{
		std::cout << game->at(0) << " " << game->at(1) << std::endl;
	}

	std::cout << tournament.tournamentDone() << std::endl;

//	PlatypusGame::printTable(167902255);

	return 0;
}
