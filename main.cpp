#include <iostream>
#include <vector>
#include "PlatypusGame.h"

int main()
{
	std::vector<unsigned long> players;

	players.push_back(167902255);
	players.push_back(180902255);

	PlatypusGame game = PlatypusGame();

	PlatypusGame::Result result = game.runGame(players, true);



//	PlatypusGame::printTable(167902255);

	return 0;
}
