
#ifndef PLATYPUSGAME_TOURNAMENT_H
#define PLATYPUSGAME_TOURNAMENT_H

#include <vector>
#include "PlatypusGame.h"

class Tournament
{
public:
	/**
	 * Check if this tournament is finished (all games played)
	 * @return True if tournament is finished, false otherwise.
	 */
	virtual bool tournamentDone() = 0;

	/**
	 * Get the next games to play in the tournament.
	 * @param count The limit for the amount of games to get.
	 * @return A vector of at most count games. The vector may be shorter than specified in count.
	 */
	virtual std::vector<std::shared_ptr<std::vector<unsigned long>>> getNextGames(int count) = 0;

	/**
	 * Process a vector of results, to create final ranking output and also potentially determine future matches.
	 * @param results A vector of Results to process. Each result must be for a game created by this Tournament.
	 */
	virtual void processResults(std::vector<std::shared_ptr<PlatypusGame::Result>> results) = 0;

	/**
	 * Output the results of the tournament. If tournament is not done, results will be incomplete.
	 * @param outputStream Stream to output the results to.
	 */
	virtual void outputResults(std::ostream& outputStream) = 0;

	/**
	 * Output a file containing the information required to resume this tournament later.
	 * @param saveStream Stream to write the file to.
	 */
	virtual void save(std::ostream saveStream) = 0;
};

#endif //PLATYPUSGAME_TOURNAMENT_H
