
#ifndef PLATYPUSGAME_SINGLEELIMINATIONTOURNAMENT_H
#define PLATYPUSGAME_SINGLEELIMINATIONTOURNAMENT_H


#include <vector>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <stack>
#include "Tournament.h"

class SingleEliminationTournament : public Tournament
{
	std::vector<unsigned long> playerIDs;

	std::stack<unsigned long> qualifiedPlayers;

	std::vector<std::shared_ptr<std::vector<unsigned long>>> gameList;

	int roundNumber;
	size_t nextGameIndex;
	size_t numResultsProcessed;

	std::mutex nextGameMutex;
	std::mutex processResultsMutex;

	std::unordered_map<unsigned long, int> standings;

	std::default_random_engine randomEngine;

	void generateNextRound();

public:
	SingleEliminationTournament(std::vector<unsigned long>& playerIDs);

	bool tournamentDone() override;

	std::vector<std::shared_ptr<std::vector<unsigned long>>> getNextGames(int count) override;

	void processResults(std::vector<std::shared_ptr<PlatypusGame::Result>> results) override;

	void outputResults(std::ostream& outputStream) override;

	void save(std::ostream saveStream) override;
};


#endif //PLATYPUSGAME_SINGLEELIMINATIONTOURNAMENT_H
