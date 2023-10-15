
#define BATCH_SIZE 1000

#include "TournamentRunner.h"
#include <vector>
#include <thread>

TournamentRunner::TournamentRunner(Tournament& tournament, PlatypusGame& platypusGame, unsigned int threadCount) :
		tournament {tournament}, platypusGame {platypusGame}, threadCount {threadCount}
{
}

void TournamentRunner::playGames()
{
	std::vector<std::shared_ptr<std::vector<unsigned long>>> games;
	std::vector<std::shared_ptr<PlatypusGame::Result>> results;

	while (!tournament.tournamentDone())
	{
		games = tournament.getNextGames(BATCH_SIZE);
		results.clear();

		for (const auto& game : games)
		{
			results.push_back(platypusGame.runGame(*game));
		}

		tournament.processResults(results);
	}
}

void TournamentRunner::runTournament()
{
	std::vector<std::thread> threads;

	threads.reserve(threadCount);
	for (int i = 0; i < threadCount; ++i)
	{
		threads.emplace_back(&TournamentRunner::playGames, this);
	}

	// Wait for threads to finish
	for (auto& thread : threads)
	{
		thread.join();
	}
}
