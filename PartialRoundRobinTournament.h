
#ifndef PLATYPUSGAME_PARTIALROUNDROBINTOURNAMENT_H
#define PLATYPUSGAME_PARTIALROUNDROBINTOURNAMENT_H

#include <vector>
#include <mutex>
#include <unordered_map>
#include "Tournament.h"

class PartialRoundRobinTournament : public Tournament
{
	struct Standing {
		long wins;
		long gamesPlayed;
		long pointsFor;
		long pointsAgainst;
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
	 * Construct a new partial round robin tournament from a vector of participating players.
	 * @param playerIDs Vector of playerIDs to use for the tournament.
	 * @param percentage Percentage of rounds to play in this round robin.
	 */
	PartialRoundRobinTournament(std::vector<unsigned long>& playerIDs, int percentage);

	PartialRoundRobinTournament(std::istream saveStream);

	bool tournamentDone() override;

	std::vector<std::shared_ptr<std::vector<unsigned long>>> getNextGames(int count) override;

	void processResults(std::vector<std::shared_ptr<PlatypusGame::Result>> results) override;

	void outputResults(std::ostream& outputStream) override;

	void save(std::ostream saveStream) override;
};


#endif //PLATYPUSGAME_PARTIALROUNDROBINTOURNAMENT_H
