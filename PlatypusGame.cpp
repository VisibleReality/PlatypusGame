
#include <array>
#include <iostream>
#include <deque>
#include "PlatypusGame.h"

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

	std::deque<Player> players;

	// Create Player structs for every player
	for (long id : playerIDs)
	{
		players.push_back(Player {id, 0, (BOARD_SIZE - 1) / 2, State::KANGAROO});
	}

	// One game for each player, so each gets a turn going first
	for (int i = 0; i < players.size(); ++i)
	{
		// Run through each turn of the game
		for (int turn = 0; turn < gameLength; ++turn)
		{
			// Run through every player in order
			for (Player player : players)
			{
				// Check to see if the player is a green platypus (terminates this game)
				if (board.at(player.position) and player.state == PLATYPUS)
				{
					break;
				}
				// Otherwise run the player's turn and update the board
				board.at(player.position) = updatePlayer(player, board.at(player.position));
			}
		}
		// Rotate list of players after every game
		players.push_back(players.front());
		players.pop_front();
	}

	// Find winning player(s) and construct vector of scores
	std::vector<int> scores;
	std::vector<long> winners;
	int winningScore = 0;
	for(Player player : players)
	{
		scores.push_back(player.score);
		if (player.score > winningScore)
		{
			winningScore = player.score;
			winners.clear();
			winners.push_back(player.id);
		}
		else if (player.score == winningScore)
		{
			winners.push_back(player.id);
		}
	}

	// If list of winners is the same length as the list of players, then it is a draw and there are no winners.
	winners.clear();

	return {winners, scores};
}

PlatypusGame::Result PlatypusGame::runGame(const std::vector<long>& playerIDs)
{
	return runGame(playerIDs, false);
}

bool PlatypusGame::updatePlayer(PlatypusGame::Player& player, bool tileColour)
{
	return false;
}

