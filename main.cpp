#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <algorithm>
#include "include/argparse/argparse.hpp"
#include "PlatypusGame.h"
#include "RoundRobinTournament.h"
#include "TournamentRunner.h"
#include "PartialRoundRobinTournament.h"
#include "EloRandomTournament.h"

int main(int argc, char* argv[])
{
	argparse::ArgumentParser program("PlatypusGame");

	program.add_argument("tournament-type")
			.help("Name of tournament to run. [x] indicates a variable. Valid names are: roundrobin partialroundrobin_[x] elorandom_[x]");
	program.add_argument("input").help("File to read machine numbers from");
	program.add_argument("output").help("Filename to write results to");

	program.add_argument("--rules")
			.help("List of rules to add to the platypus game. Valid rules are: short long tree tiebreaker terminate")
			.nargs(argparse::nargs_pattern::at_least_one);
	program.add_argument("--game-length")
			.help("Set custom game length, in turns")
			.scan<'d', int>();

	try
	{
		program.parse_args(argc, argv);
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
		std::cerr << program;
		std::exit(1);
	}


	std::vector<unsigned long> players;

	std::ifstream inFile(program.get<std::string>("input"));
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

	if (players.empty())
	{
		std::cerr << "Error: Could not load any players" << std::endl;
		std::exit(1);
	}

	std::cout << "Loaded " << players.size() << " players" << std::endl;

	std::vector<PlatypusGame::Rule> rules;
	if (program.present("--rules"))
	{
		std::cout << "Using rules: ";
		for (const auto& rule : program.get<std::vector<std::string>>("--rules"))
		{
			if (rule == "long")
			{
				rules.push_back(PlatypusGame::Rule::LONG);
				std::cout << "long";
			}
			else if (rule == "short")
			{
				rules.push_back(PlatypusGame::Rule::SHORT);
				std::cout << "short ";
			}
			else if (rule == "tree")
			{
				rules.push_back(PlatypusGame::Rule::TREE);
				std::cout << "tree ";
			}
			else if (rule == "tiebreaker")
			{
				rules.push_back(PlatypusGame::Rule::TIEBREAKER);
				std::cout << "tiebreaker ";
			}
			else if (rule == "terminate")
			{
				rules.push_back(PlatypusGame::Rule::TERMINATE);
				std::cout << "terminate ";
			}
		}
		std::cout << std::endl;
	}

	auto startTime = std::chrono::high_resolution_clock::now();

	PlatypusGame platypusGame;
	if (program.present("--game-length"))
	{
		platypusGame = PlatypusGame(rules, program.get<int>("--game-length"));
		std::cout << "Using game length: " << program.get<int>("--game-length") << std::endl;
	}
	else
	{
		platypusGame = PlatypusGame(rules);
	}

	std::shared_ptr<Tournament> tournament;

	auto tournamentType = program.get<std::string>("tournament-type");
	if (tournamentType == "roundrobin")
	{
		std::cout << "Using tournament type: " << tournamentType << std::endl;
		tournament = std::make_shared<RoundRobinTournament>(players);
	}
	else if (tournamentType.rfind("partialroundrobin", 0) == 0)
	{
		std::cout << "Using tournament type: " << tournamentType << std::endl;
		tournamentType.erase(std::remove_if(tournamentType.begin(), tournamentType.end(),
											[](auto const& c) -> bool { return !std::isdigit(c); }),
							 tournamentType.end());

		if (tournamentType.empty())
		{
			std::cerr << "Error: Must specify percentage for partial round robin tournament." << std::endl;
			std::exit(1);
		}
		tournament = std::make_shared<PartialRoundRobinTournament>(players, std::stoi(tournamentType));
	}
	else if (tournamentType.rfind("elorandom", 0) == 0)
	{
		std::cout << "Using tournament type: " << tournamentType << std::endl;
		tournamentType.erase(std::remove_if(tournamentType.begin(), tournamentType.end(),
											[](auto const& c) -> bool { return !std::isdigit(c); }),
							 tournamentType.end());

		if (tournamentType.empty())
		{
			std::cerr << "Error: Must specify percentage for random elo tournament." << std::endl;
			std::exit(1);
		}
		tournament = std::make_shared<EloRandomTournament>(players, std::stoi(tournamentType));
	}
	else
	{
		std::cerr << "Error: Unknown tournament type" << std::endl;
		std::exit(1);
	}

	TournamentRunner runner = TournamentRunner(*tournament, platypusGame,
											   std::thread::hardware_concurrency());

	runner.runTournament();

	auto finishTime = std::chrono::high_resolution_clock::now();

	std::ofstream outFile(program.get<std::string>("output"));

	tournament->outputResults(outFile);

	std::chrono::duration<double, std::milli> duration = finishTime - startTime;

	std::cout << "Took " << duration.count() / 1000 << " seconds" << std::endl;

	return 0;
}
