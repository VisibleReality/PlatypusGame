
#ifndef PLATYPUSGAME_TOURNAMENTRUNNER_H
#define PLATYPUSGAME_TOURNAMENTRUNNER_H


#include "Tournament.h"
#include "PlatypusGame.h"

class TournamentRunner
{
	Tournament& tournament;
	PlatypusGame& platypusGame;
	unsigned int threadCount;

	void playGames();

public:
	TournamentRunner(Tournament& tournament, PlatypusGame& platypusGame, unsigned int threadCount);

	void runTournament();
};


#endif //PLATYPUSGAME_TOURNAMENTRUNNER_H
