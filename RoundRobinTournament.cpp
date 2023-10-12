
#include <istream>
#include "RoundRobinTournament.h"

RoundRobinTournament::RoundRobinTournament(std::vector<unsigned long>& playerIDs) :
		playerIDs {playerIDs}, nextGameIndex {0}
{
	for (size_t i = 0; i < playerIDs.size(); ++i)
	{
		for (size_t j = i; j < playerIDs.size(); ++j)
		{
			gameList.push_back(std::make_shared<std::vector<unsigned long>>(
					std::vector<unsigned long> {playerIDs.at(i), playerIDs.at(j)}
			));
		}

		standings.at(playerIDs.at(i)) = Standing({0, 0, 0});
	}
}

RoundRobinTournament::RoundRobinTournament(std::istream saveStream)
{
	throw std::logic_error("Not Implemented");
}

bool RoundRobinTournament::tournamentDone()
{
	return nextGameIndex == gameList.size();
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
		for (auto winner : result->winners)
		{
			++standings.at(winner).wins;
		}

		for (size_t i = 0; i < result->participants.size(); ++i)
		{
			standings.at(result->participants.at(i)).pointsFor += result->scores.at(i);
			for (auto participant : result->participants)
			{
				if (participant != result->participants.at(i))
				{
					standings.at(participant).pointsAgainst += result->scores.at(i);
				}
			}
		}
	}
}

void RoundRobinTournament::outputResults(std::ostream outputStream)
{

}

void RoundRobinTournament::save(std::ostream saveStream)
{
	throw std::logic_error("Not Implemented");
}

