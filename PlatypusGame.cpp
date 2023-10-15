
#include <array>
#include <iostream>
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

std::shared_ptr<PlatypusGame::Result> PlatypusGame::runGame
		(const std::vector<unsigned long>& playerIDs, bool printTrace)
{
	std::array<bool, BOARD_SIZE> board {false};

	std::deque<std::shared_ptr<Player>> players;

	// Create Player structs for every player
	for (unsigned long id : playerIDs)
	{
		players.push_back(std::make_shared<Player>(Player {id, 0, (BOARD_SIZE - 1) / 2, KANGAROO}));
	}

	// One game for each player, so each gets a turn going first
	for (int i = 0; i < players.size(); ++i)
	{
		if (printTrace)
		{
			std::cout << "Game " << i + 1 << std::endl;
		}

		// Run through each turn of the game
		for (int turn = 0; turn < gameLength / players.size(); ++turn)
		{
			bool terminated = false;
			// Run through every player in order
			for (const std::shared_ptr<Player>& player : players)
			{
				if (printTrace)
				{
					std::cout << "Turn " << turn + 1 << " Player " << player->id << std::endl;
					printBoard(board, players);
				}

				// Check to see if the player is a green platypus (terminates this game)
				if (board.at(player->position) and player->state == PLATYPUS)
				{
					if (printTrace)
					{
						std::cout << "Game terminated by " << player->id << std::endl;
						std::cout << "Current scores:" << std::endl;
						for (const std::shared_ptr<Player>& player_ : players)
						{
							std::printf("M%9ld: %4d\n", player_->id, player_->score);
						}
					}

					terminated = true;
					break;
				}

				// Otherwise run the player's turn and update the board
				size_t originalPosition = player->position;
				board.at(originalPosition) = updatePlayer(player, board.at(originalPosition));
			}

			if (terminated)
			{
				break;
			}
		}
		// Rotate list of players after every game
		players.push_back(players.front());
		players.pop_front();

		// Reset board and players
		board.fill(false);
		for (const std::shared_ptr<Player>& player : players)
		{
			player->position = (BOARD_SIZE - 1) / 2;
			player->state = KANGAROO;
		}
	}

	// Find winning player(s) and construct vector of scores
	std::vector<int> scores;
	std::vector<unsigned long> winners;
	int winningScore = 0;
	for (const std::shared_ptr<Player>& player : players)
	{
		scores.push_back(player->score);
		if (player->score > winningScore)
		{
			winningScore = player->score;
			winners.clear();
			winners.push_back(player->id);
		} else if (player->score == winningScore)
		{
			winners.push_back(player->id);
		}
	}

	// If list of winners is the same length as the list of players, then it is a draw and there are no winners.
	if (winners.size() == players.size())
	{
		winners.clear();
	}

	if (printTrace)
	{
		std::cout << "Winners:\n";
		for (unsigned long winner : winners)
		{
			std::cout << winner << "\n";
		}
		std::cout << "Scores:\n";
		for (int i = 0; i < players.size(); ++i)
		{
			std::cout << players.at(i)->id << ": " << scores.at(i) << "\n";
		}
		std::cout << std::endl;
	}

	auto result = std::make_shared<PlatypusGame::Result>();
	result->participants = playerIDs;
	result->winners = winners;
	result->scores = scores;

	return result;
}

std::shared_ptr<PlatypusGame::Result> PlatypusGame::runGame(const std::vector<unsigned long>& playerIDs)
{
	return runGame(playerIDs, false);
}

bool PlatypusGame::updatePlayer(const std::shared_ptr<Player>& player, bool tileColour)
{
	// Bit-shifting magic code

	std::bitset<28> idBitset = (player->id - 1);

	int offset = 6 - (((int) player->state << 1) + ((int) tileColour));

	idBitset >>= offset;

	if (idBitset.test(0)) // If moving to the left
	{
		player->position -= 1;
		if (player->position < 0)
		{ // If moved off left side, move to right
			player->position = BOARD_SIZE - 1;
		}
	} else // Otherwise move to the right
	{
		player->position += 1;
		if (player->position >= BOARD_SIZE)
		{ // If moved off right side, move to left
			player->position = 0;
		}
	}

	idBitset >>= 7 + offset;

	// Set new state
	switch ((idBitset & std::bitset<28>(3)).to_ulong())
	{
		case 0:
			player->state = KANGAROO;
			break;
		case 1:
			player->state = EMU;
			break;
		case 2:
			player->state = WOMBAT;
			break;
		case 3:
			player->state = PLATYPUS;
			break;
	}

	idBitset >>= 14 - offset;

	bool newColour = idBitset.test(0);
	if (newColour != tileColour)
	{
		player->score += 1;
	}

	// Return new colour
	return newColour;
}

void PlatypusGame::printBoard(std::array<bool, 21>& board, std::deque<std::shared_ptr<Player>>& players)
{
	for (const std::shared_ptr<Player>& player : players)
	{
		std::string spacer(player->position, ' ');
		std::printf("M%9ld (%4d): %s%c\n", player->id, player->score, spacer.data(), stateToChar(player->state));
	}
	std::cout << "            Board: ";
	for (bool tile : board)
	{
		if (tile)
		{
			std::cout << 'G';
		} else
		{
			std::cout << "Y";
		}
	}
	std::cout << std::endl;
}

void PlatypusGame::printTable(unsigned long playerID)
{
	std::cout << "| Y  | G  | Y  | G  | Y  | G  | Y  |\n";
	std::cout << "| K  | K  | E  | E  | W  | W  | P  |\n";
	std::cout << "|----|----|----|----|----|----|----|\n";

	std::vector<bool> newColours;
	std::vector<State> newStates;
	std::vector<bool> directions;

	for (State state : std::array<State, 4> {KANGAROO, EMU, WOMBAT, PLATYPUS})
	{
		for (bool colour : std::array<bool, 2> {false, true})
		{
			if (!(state == PLATYPUS && colour))
			{
				std::shared_ptr<Player> player = std::make_shared<Player>(Player {playerID, 0, 1, state});
				newColours.push_back(updatePlayer(player, colour));
				newStates.push_back(player->state);
				directions.push_back(player->position > 0);
			}
		}
	}

	std::cout << "|";
	for (bool colour : newColours)
	{
		if (colour)
		{
			std::cout << " G  |";
		} else
		{
			std::cout << " Y  |";
		}
	}
	std::cout << "\n";

	std::cout << "|";
	for (State state : newStates)
	{
		std::cout << " " << stateToChar(state) << "  |";
	}
	std::cout << "\n";

	std::cout << "|";
	for (bool direction : directions)
	{
		if (direction)
		{
			std::cout << " GG |";
		} else
		{
			std::cout << " WA |";
		}
	}
	std::cout << std::endl;
}

char PlatypusGame::stateToChar(PlatypusGame::State state)
{
	switch (state)
	{
		case KANGAROO:
			return 'K';
		case EMU:
			return 'E';
		case WOMBAT:
			return 'W';
		case PLATYPUS:
			return 'P';
	}
	return '?';
}
