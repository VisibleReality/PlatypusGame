
#include <array>
#include <iostream>
#include "PlatypusGame.h"

enum State
{
	KANGAROO,
	EMU,
	WOMBAT,
	PLATYPUS
};

struct Player
{
	long id;
	int score;
	long long position;
	State state;
};

PlatypusGame::PlatypusGame() : PlatypusGame {std::vector<Rule>(), STANDARD_GAME_TURNS}
{}

PlatypusGame::PlatypusGame(const std::vector<Rule>& rules) : PlatypusGame {rules, STANDARD_GAME_TURNS}
{
	for (Rule rule : rules)
	{
		if (rule == Rule::LONG)
		{
			gameLength = LONG_GAME_TURNS;
		} else if (rule == Rule::SHORT)
		{
			gameLength = SHORT_GAME_TURNS;
		}
	}
}

PlatypusGame::PlatypusGame(const std::vector<Rule>& rules, int gameLength) : gameLength {gameLength}
{
	for (Rule rule : rules)
	{
		// Set rule flags
		// No rules implemented yet
	}
}

PlatypusGame::Result PlatypusGame::runGame(const std::vector<long>& playerIDs, bool printTrace)
{
	std::array<bool, BOARD_SIZE> board {false};

	std::vector<Player> players;

	// Create Player structs for every player
	players.reserve(playerIDs.size());
	for (long id : playerIDs)
	{
		players.push_back(Player {id, 0, (BOARD_SIZE - 1) / 2, State::KANGAROO});
	}

	for (Player player : players)
	{
		std::cout << player.id << player.position << player.state << std::endl;
	}

	return {-1, std::vector<int> {}};
}

PlatypusGame::Result PlatypusGame::runGame(const std::vector<long>& playerIDs)
{
	return runGame(playerIDs, false);
}

