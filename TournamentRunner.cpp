
#define BATCH_SIZE 1000

#include "TournamentRunner.h"
#include <vector>
#include <thread>
//#include <iostream>

TournamentRunner::TournamentRunner(Tournament& tournament, PlatypusGame& platypusGame, unsigned int threadCount) :
		tournament {tournament}, platypusGame {platypusGame}, threadCount {threadCount}
{
}

void TournamentRunner::playGames()
{
//	std::cout << "Starting thread" << std::endl;
	std::vector<std::shared_ptr<std::vector<unsigned long>>> games;
	std::vector<std::shared_ptr<PlatypusGame::Result>> results;

	while (!tournament.tournamentDone())
	{
		games = tournament.getNextGames(BATCH_SIZE);
		results.clear();

		if (games.empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

//		std::cout << "Got " << games.size() << " games" << std::endl;

		for (const auto& game : games)
		{
			results.push_back(platypusGame.runGame(*game));
		}

		tournament.processResults(results);
	}
//	std::cout << "Ending thread" << std::endl;
}

void TournamentRunner::runTournament()
{
	if (threadCount > 1)
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
	} else // Don't bother creating another thread if only 1 thread is needed
	{
		playGames();
	}
}
