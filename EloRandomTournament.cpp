
#include <istream>
#include <random>
#include <chrono>
#include <algorithm>
#include <iostream>
#include "EloRandomTournament.h"

EloRandomTournament::EloRandomTournament(std::vector<unsigned long>& playerIDs, int percentage) :
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

	config = Elo::Configuration((double) K_SCALE / ((double)gameList.size() + K_OFFSET));

	std::cout << gameList.size() << std::endl;
	std::cout << (double) K_SCALE / ((double)gameList.size() + K_OFFSET) << std::endl;
	std::cout << 1920000.0 / (double)gameList.size() << std::endl;

	for (auto playerID : playerIDs)
	{
		standings.emplace(playerID, Standing({0, 0, 0, 0, Elo::Player(STARTING_RATING, config)}));
	}
}

EloRandomTournament::EloRandomTournament(std::istream saveStream) : config {32}
{
	throw std::logic_error("Not Implemented");
}

bool EloRandomTournament::tournamentDone()
{
	return nextGameIndex == gameList.size() && numResultsProcessed == gameList.size();
}

std::vector<std::shared_ptr<std::vector<unsigned long>>> EloRandomTournament::getNextGames(int count)
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

void EloRandomTournament::processResults(std::vector<std::shared_ptr<PlatypusGame::Result>> results)
{
	// Lock so only one thread can process results at a time
	std::lock_guard<std::mutex> lock(processResultsMutex);

	for (const auto& result : results)
	{
		++numResultsProcessed;

		if (!result->winners.empty())
		{
			++standings.at(result->winners.at(0)).wins;

			if (result->winners.at(0) == result->participants.at(0))
			{
				Elo::Match
						(standings.at(result->participants.at(0)).elo,
						 standings.at(result->participants.at(1)).elo,
						 Elo::WIN).apply();
			}
			else
			{
				Elo::Match
						(standings.at(result->participants.at(0)).elo,
						 standings.at(result->participants.at(1)).elo,
						 Elo::LOSS).apply();
			}
		}
		else
		{
			Elo::Match
					(standings.at(result->participants.at(0)).elo,
					 standings.at(result->participants.at(1)).elo,
					 Elo::DRAW).apply();
		}

		unsigned long player1 = result->participants.at(0);
		unsigned long player2 = result->participants.at(1);

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

void EloRandomTournament::outputResults(std::ostream& outputStream)
{
	outputStream << "Number,Wins,Played,For,Against,Elo\n";
	for (auto playerID : playerIDs)
	{
		outputStream << playerID << ",";
		outputStream << standings.at(playerID).wins << ",";
		outputStream << standings.at(playerID).gamesPlayed << ",";
		outputStream << standings.at(playerID).pointsFor << ",";
		outputStream << standings.at(playerID).pointsAgainst << ",";
		outputStream << standings.at(playerID).elo.rating << std::endl;
	}
}

void EloRandomTournament::save(std::ostream saveStream)
{
	throw std::logic_error("Not Implemented");
}

