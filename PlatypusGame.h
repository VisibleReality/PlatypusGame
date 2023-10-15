
#ifndef PLATYPUSGAME_PLATYPUSGAME_H
#define PLATYPUSGAME_PLATYPUSGAME_H

#define BOARD_SIZE 21

#define STANDARD_GAME_TURNS 100
#define SHORT_GAME_TURNS 100
#define LONG_GAME_TURNS 200

#define TREE_BONUS 5
#define TERMINATE_BONUS 25

#include <vector>
#include <deque>
#include <memory>
#include <bitset>
#include <random>

class PlatypusGame
{
	// Rule flags
	bool ruleTree = false;
	bool ruleTiebreaker = false;
	bool ruleTerminate = false;

	std::default_random_engine randomEngine;

	int gameLength;

	enum State
	{
		KANGAROO = 0,
		EMU = 1,
		WOMBAT = 2,
		PLATYPUS = 3
	};

	/**
	 * Convert a State to a char
	 * @param state The state to convert
	 * @return 'K' for KANGAROO, 'E' for EMU, 'W' for WOMBAT, 'P' for PLATYPUS
	 */
	inline static char stateToChar(State state);

	struct Player
	{
		unsigned long id;
		int score;
		long long position;
		State state;
	};

	/**
	 * Update the given player and return the new tile colour
	 * @param player The player to update
	 * @param tileColour The current tile colour (yellow = false, green = true)
	 * @return The new tile colour (yellow = false, green = true)
	 */
	bool updatePlayer(const std::shared_ptr<Player>& player, bool tileColour) const;

	/**
	 * Print the given board state with players
	 * @param board The board state to print
	 * @param players The player positions to print
	 */
	static void printBoard(std::array<bool, BOARD_SIZE>& board, std::deque<std::shared_ptr<Player>>& players);

public:

	enum Rule
	{
		LONG,
		SHORT,
		TREE,
		TIEBREAKER,
		TERMINATE
	};

	struct Result
	{
		std::vector<unsigned long> participants;
		std::vector<unsigned long> winners;
		std::vector<int> scores;
	};

	/**
	 * Construct a new game with standard rules
	 */
	PlatypusGame();

	/**
	 * Construct a new game with specified rules
	 * @param rules Vector of rules to use for this game
	 */
	PlatypusGame(const std::vector<Rule>& rules);

	/**
	 * Construct a new game with specified rules and custom length
	 * @param rules Vector of rules to use for this game
	 * @param gameLength Game length, in turns
	 */
	PlatypusGame(const std::vector<Rule>& rules, int gameLength);

	/**
	 * Run a game with the specified players and return the result
	 * @param playerIDs A vector of the IDs of the machines in this game
	 * @return A result struct containing a vector of winners and a vector of scores
	 */
	std::shared_ptr<Result> runGame(const std::vector<unsigned long>& playerIDs);

	/**
	 * Run a game with the specified players and return the result
	 * @param playerIDs A vector of the IDs of the machines in this game
	 * @param printTrace True to print every step of the game
	 * @return A result struct containing a vector of winners and a vector of scores
	 */
	std::shared_ptr<Result> runGame(const std::vector<unsigned long>& playerIDs, bool printTrace);

	/**
	 * Run a game with the specified players and return the result
	 * @param playerIDs A vector of the IDs of the machines in this game
	 * @param printTrace True to print every step of the game
	 * @param tiebreaker Start with random board state, do not start another tiebreaker
	 * @return A result struct containing a vector of winners and a vector of scores
	 */
	std::shared_ptr<Result> runGame(const std::vector<unsigned long>& playerIDs, bool printTrace, bool tiebreaker);

	/**
	 * Print a rules table for a given machine
	 * @param playerID The ID of the machine to print
	 */
	void printTable(unsigned long playerID);
};


#endif //PLATYPUSGAME_PLATYPUSGAME_H
