
#ifndef PLATYPUSGAME_ELORANDOMTOURNAMENT_H
#define PLATYPUSGAME_ELORANDOMTOURNAMENT_H

#define K_SCALE 2024237
#define K_OFFSET 23237
#define STARTING_RATING 1600

#include <vector>
#include <mutex>
#include <unordered_map>
#include "Tournament.h"
#include "include/elo/elo.hpp"

class EloRandomTournament : public Tournament
{
	Elo::Configuration config;

	struct Standing {
		long wins{};
		long gamesPlayed{};
		long pointsFor{};
		long pointsAgainst{};
		Elo::Player elo;
	};

	std::vector<unsigned long> playerIDs;
	std::vector<std::shared_ptr<std::vector<unsigned long>>> gameList;

	size_t nextGameIndex;
	size_t numResultsProcessed;

	std::mutex nextGameMutex;
	std::mutex processResultsMutex;

	std::unordered_map<unsigned long, Standing> standings;


public:
	/**
	 * Construct a new elo tournament with random pairings.
	 * @param playerIDs Vector of playerIDs to use for the tournament.
	 * @param percentage Percentage of rounds to play in this round robin.
	 */
	EloRandomTournament(std::vector<unsigned long>& playerIDs, int percentage);

	EloRandomTournament(std::istream saveStream);

	bool tournamentDone() override;

	std::vector<std::shared_ptr<std::vector<unsigned long>>> getNextGames(int count) override;

	void processResults(std::vector<std::shared_ptr<PlatypusGame::Result>> results) override;

	void outputResults(std::ostream& outputStream) override;

	void save(std::ostream saveStream) override;
};


#endif //PLATYPUSGAME_ELORANDOMTOURNAMENT_H
