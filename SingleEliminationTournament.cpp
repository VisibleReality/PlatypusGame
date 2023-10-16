
#include <stdexcept>
#include <algorithm>
#include <ostream>
#include "SingleEliminationTournament.h"


SingleEliminationTournament::SingleEliminationTournament(std::vector<unsigned long>& playerIDs) :
		playerIDs {playerIDs}, roundNumber {0}, nextGameIndex {0}, numResultsProcessed {0}
{
	randomEngine = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

	auto shuffledPlayerIDs = std::vector(playerIDs);

	std::shuffle(shuffledPlayerIDs.begin(), shuffledPlayerIDs.end(), randomEngine);

	for (auto playerID : shuffledPlayerIDs)
	{
		qualifiedPlayers.push(playerID);
	}

	auto targetPlayers = (size_t) std::exp2(std::floor(std::log2(playerIDs.size())));

	for (size_t i = 0; i < (playerIDs.size() - targetPlayers); ++i)
	{
		unsigned long player1 = qualifiedPlayers.top();
		qualifiedPlayers.pop();
		unsigned long player2 = qualifiedPlayers.top();
		qualifiedPlayers.pop();

		gameList.push_back(std::make_shared<std::vector<unsigned long>>(std::vector<unsigned long> {player1, player2}));
	}
}

bool SingleEliminationTournament::tournamentDone()
{
	// Lock so we don't check when a new round might be generating
	std::lock_guard<std::mutex> lock(nextGameMutex);

	return gameList.empty();
}

std::vector<std::shared_ptr<std::vector<unsigned long>>> SingleEliminationTournament::getNextGames(int count)
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

void SingleEliminationTournament::processResults(std::vector<std::shared_ptr<PlatypusGame::Result>> results)
{
	// Lock so only one thread can process results at a time
	std::lock_guard<std::mutex> lock(processResultsMutex);

	auto coinflip = std::uniform_int_distribution<>(0, 1);

	for (const auto& result : results)
	{
		++numResultsProcessed;

		if (result->winners.empty()) // Flip coin in case of ties (because we must have a conclusive result)
		{
			result->winners.push_back(result->participants.at(coinflip(randomEngine)));
		}

		if (result->winners.at(0) == result->participants.at(0))
		{
			qualifiedPlayers.push(result->participants.at(0));
			standings.emplace(result->participants.at(1), roundNumber);
		}
		else
		{
			qualifiedPlayers.push(result->participants.at(1));
			standings.emplace(result->participants.at(0), roundNumber);
		}
	}

	if (numResultsProcessed == gameList.size())
	{
		generateNextRound();
	}
}

void SingleEliminationTournament::outputResults(std::ostream& outputStream)
{
	outputStream << "Number,RoundEliminated\n";
	for (auto playerID : playerIDs)
	{
		outputStream << playerID << ",";
		outputStream << standings.at(playerID) << std::endl;
	}
}

void SingleEliminationTournament::save(std::ostream saveStream)
{
	throw std::logic_error("Not implemented");
}

void SingleEliminationTournament::generateNextRound()
{
	// Lock so only no thread can get games while we generate them
	std::lock_guard<std::mutex> lock(nextGameMutex);

	roundNumber += 1;
	gameList.clear();
	nextGameIndex = 0;
	numResultsProcessed = 0;

	if (qualifiedPlayers.size() == 1) // We have a winner!
	{
		standings.emplace(qualifiedPlayers.top(), roundNumber);
		return;
	}

	while (!qualifiedPlayers.empty())
	{
		unsigned long player1 = qualifiedPlayers.top();
		qualifiedPlayers.pop();
		unsigned long player2 = qualifiedPlayers.top();
		qualifiedPlayers.pop();

		gameList.push_back(std::make_shared<std::vector<unsigned long>>(std::vector<unsigned long> {player1, player2}));
	}
}
