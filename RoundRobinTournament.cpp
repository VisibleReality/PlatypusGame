
#include <istream>
#include "RoundRobinTournament.h"

RoundRobinTournament::RoundRobinTournament(std::vector<unsigned long>& playerIDs) :
		playerIDs {playerIDs}, nextGameIndex {0}, numResultsProcessed {0}
{
	for (size_t i = 0; i < playerIDs.size(); ++i)
	{
		for (size_t j = i; j < playerIDs.size(); ++j)
		{
			gameList.push_back(std::make_shared<std::vector<unsigned long>>(
					std::vector<unsigned long> {playerIDs.at(i), playerIDs.at(j)}
			));
		}

		standings.emplace(playerIDs.at(i), Standing({0, 0, 0}));
	}
}

RoundRobinTournament::RoundRobinTournament(std::istream saveStream)
{
	throw std::logic_error("Not Implemented");
}

bool RoundRobinTournament::tournamentDone()
{
	return nextGameIndex == gameList.size() && numResultsProcessed == gameList.size();
}

std::vector<std::shared_ptr<std::vector<unsigned long>>> RoundRobinTournament::getNextGames(int count)
{
	// Lock so only one thread can get games at a time
	std::lock_guard<std::mutex> lock(nextGameMutex);

	auto nextGames = std::vector<std::shared_ptr<std::vector<unsigned long>>>(
					gameList.begin() + nextGameIndex,
					nextGameIndex + count < gameList.size() ? gameList.begin() + nextGameIndex + count
															: gameList.end());

	nextGameIndex = nextGameIndex + count < gameList.size() ? nextGameIndex + count : gameList.size();
	return nextGames;
}

void RoundRobinTournament::processResults(std::vector<std::shared_ptr<PlatypusGame::Result>> results)
{
	// Lock so only one thread can process results at a time
	std::lock_guard<std::mutex> lock(processResultsMutex);

	for (const auto& result : results)
	{
		++numResultsProcessed;

		if (!result->winners.empty())
		{
			++standings.at(result->winners.at(0)).wins;
		}

		unsigned long player1 = result->participants.at(0);
		unsigned long player2 = result->participants.at(1);

		standings.at(player1).pointsFor += result->scores.at(0);
		standings.at(player2).pointsAgainst += result->scores.at(0);

		standings.at(player2).pointsFor += result->scores.at(1);
		standings.at(player1).pointsAgainst += result->scores.at(1);
	}
}

void RoundRobinTournament::outputResults(std::ostream& outputStream)
{
	outputStream << "Number,Wins,For,Against\n";
	for (auto playerID : playerIDs)
	{
		outputStream << playerID << ",";
		outputStream << standings.at(playerID).wins << ",";
		outputStream << standings.at(playerID).pointsFor << ",";
		outputStream << standings.at(playerID).pointsAgainst << std::endl;
	}
}

void RoundRobinTournament::save(std::ostream saveStream)
{
	throw std::logic_error("Not Implemented");
}

