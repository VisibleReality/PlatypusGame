
#include <istream>
#include <random>
#include <chrono>
#include <algorithm>
#include "GlickoRandomTournament.h"

GlickoRandomTournament::GlickoRandomTournament(std::vector<unsigned long>& playerIDs, int percentage) :
		playerIDs {playerIDs}, nextGameIndex {0}, numResultsProcessed {0}
{
	std::vector<std::shared_ptr<std::vector<unsigned long>>> fullGameList;

	for (size_t i = 0; i < playerIDs.size(); ++i)
	{
		for (size_t j = i; j < playerIDs.size(); ++j)
		{
			fullGameList.push_back(std::make_shared<std::vector<unsigned long>>(
					std::vector<unsigned long> {playerIDs.at(i), playerIDs.at(j)}
			));
		}
	}

	auto randomEngine = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle(fullGameList.begin(), fullGameList.end(), randomEngine);

	gameList = std::vector<std::shared_ptr<std::vector<unsigned long>>>
			(fullGameList.begin(), fullGameList.begin() + fullGameList.size() * (percentage / 100.0));

	for (auto playerID : playerIDs)
	{
		standings.emplace(playerID, Standing({0, 0, 0, 0, Glicko::Rating()}));
	}
}

GlickoRandomTournament::GlickoRandomTournament(std::istream saveStream)
{
	throw std::logic_error("Not Implemented");
}

bool GlickoRandomTournament::tournamentDone()
{
	return nextGameIndex == gameList.size() && numResultsProcessed == gameList.size();
}

std::vector<std::shared_ptr<std::vector<unsigned long>>> GlickoRandomTournament::getNextGames(int count)
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

void GlickoRandomTournament::processResults(std::vector<std::shared_ptr<PlatypusGame::Result>> results)
{
	// Lock so only one thread can process results at a time
	std::lock_guard<std::mutex> lock(processResultsMutex);

	for (const auto& result : results)
	{
		++numResultsProcessed;

		unsigned long player1 = result->participants.at(0);
		unsigned long player2 = result->participants.at(1);

		if (!result->winners.empty())
		{
			++standings.at(result->winners.at(0)).wins;

			if (result->winners.at(0) == player1)
			{
				standings.at(player1).glicko.Update(player2, 1.0);
				standings.at(player2).glicko.Update(player1, 0.0);
				standings.at(player1).glicko.Apply();
				standings.at(player2).glicko.Apply();
			}
			else
			{
				standings.at(player1).glicko.Update(player2, 0.0);
				standings.at(player2).glicko.Update(player1, 1.0);
				standings.at(player1).glicko.Apply();
				standings.at(player2).glicko.Apply();
			}
		}
		else
		{
			standings.at(player1).glicko.Update(player2, 0.5);
			standings.at(player2).glicko.Update(player1, 0.5);
			standings.at(player1).glicko.Apply();
			standings.at(player2).glicko.Apply();
		}

		standings.at(player1).pointsFor += result->scores.at(0);
		standings.at(player2).pointsAgainst += result->scores.at(0);

		standings.at(player2).pointsFor += result->scores.at(1);
		standings.at(player1).pointsAgainst += result->scores.at(1);

		++standings.at(player1).gamesPlayed;
		if (player1 != player2)
		{
			++standings.at(player2).gamesPlayed;
		}
	}
}

void GlickoRandomTournament::outputResults(std::ostream& outputStream)
{
	outputStream << "Number,Wins,Played,For,Against,Rating,Deviation\n";
	for (auto playerID : playerIDs)
	{
		outputStream << playerID << ",";
		outputStream << standings.at(playerID).wins << ",";
		outputStream << standings.at(playerID).gamesPlayed << ",";
		outputStream << standings.at(playerID).pointsFor << ",";
		outputStream << standings.at(playerID).pointsAgainst << ",";
		outputStream << standings.at(playerID).glicko.Rating1() << ",";
		outputStream << standings.at(playerID).glicko.Deviation1() << std::endl;
	}
}

void GlickoRandomTournament::save(std::ostream saveStream)
{
	throw std::logic_error("Not Implemented");
}

