#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <algorithm>
#include <cctype>
#include "PlatypusGame.h"
#include "RoundRobinTournament.h"
#include "TournamentRunner.h"

int main()
{
	std::vector<unsigned long> players;

	std::ifstream inFile("players.txt");
	std::string line;

	while (std::getline(inFile, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(),
								  [](auto const& c) -> bool { return !std::isdigit(c); }),
				   line.end());
		if (!line.empty())
		{
			players.push_back(std::stoi(line));
		}
	}

	std::cout << "Loaded " << players.size() << " players" << std::endl;

	RoundRobinTournament tournament = RoundRobinTournament(players);

	PlatypusGame platypusGame = PlatypusGame(
			{PlatypusGame::Rule::TREE, PlatypusGame::Rule::TERMINATE, PlatypusGame::Rule::TIEBREAKER});

	TournamentRunner runner = TournamentRunner(tournament, platypusGame,
											   std::thread::hardware_concurrency());

	auto startTime = std::chrono::high_resolution_clock::now();

	runner.runTournament();

	auto finishTime = std::chrono::high_resolution_clock::now();

	std::ofstream outFile("results.csv");

	tournament.outputResults(outFile);

	std::chrono::duration<double, std::milli> duration = finishTime - startTime;

	std::cout << "Took " << duration.count() / 1000 << " seconds" << std::endl;

	return 0;
}
