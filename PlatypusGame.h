
#ifndef PLATYPUSGAME_PLATYPUSGAME_H
#define PLATYPUSGAME_PLATYPUSGAME_H

#define BOARD_SIZE 21
#define STANDARD_GAME_TURNS 100
#define SHORT_GAME_TURNS 100
#define LONG_GAME_TURNS 200

#include <vector>

class PlatypusGame
{
	// Rule flags
	//todo add rules

	int gameLength;

public:

	enum Rule
	{
		LONG,
		SHORT
		// TODO add other rules
	};

	struct Result
	{
		int winner;
		std::vector<int> scores;
	};

	/**
	 * @brief Construct a new game with standard rules
	 */
	PlatypusGame();

	/**
	 * @brief Construct a new game with rules
	 */
	PlatypusGame(const std::vector<Rule>&);

	/**
	 * @brief Construct a new game with rules and custom game length
	 */
	PlatypusGame(const std::vector<Rule>&, int);

	/**
	 * @brief Run the game and return a result
	 */
	Result runGame(const std::vector<long>&);

	/**
	 * @brief Run the game and return a result (print each step of the game)
	 */
	Result runGame(const std::vector<long>&, bool);
};


#endif //PLATYPUSGAME_PLATYPUSGAME_H
