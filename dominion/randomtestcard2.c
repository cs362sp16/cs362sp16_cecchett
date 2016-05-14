#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "softassert.h"

#define TEST_COUNT 100000

int buys;
int handCounts[MAX_PLAYERS];

void remember(struct gameState* g) {
	// remember how many buys the player had
	buys = g->numBuys;

	// remember how many cards each player
	for(int i = 0; i < g->numPlayers; i++) {
		handCounts[i] = g->handCount[i];
	}
}

void verifyBehavior(struct gameState* g) {
	// check if numBuys was incremented
	softassert(g->numBuys == buys + 1, "numBuys was not incremented by 1.");

	// hand count should go up by 1 for all players except current, by 3 for current
	for(int i = 0; i < g->numPlayers; i++) {
		softassert(g->handCount[i] == handCounts[i] + (i == g->whoseTurn ? 3 : 1), "Hand count was not correctly incremented for a player.");
	}
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Please enter exactly one argument to specify the random seed. ./randomtest 42\n");
		return -1;
	}

	srand(atoi(argv[1]));

	for(int i = 0; i < TEST_COUNT; i++) {
		// initialize a new gameState
		struct gameState g;

		int playerCount = rand() % 3 + 2;

		int k[10] = {smithy,adventurer,gardens,embargo,cutpurse,mine,ambassador,
			outpost,baron,tribute};

		softassert(initializeGame(playerCount, k, rand(), &g) == 0, "Game initialization failed.");

		// set all hands to random cards
		for(int i = 0; i < g.numPlayers; i++) {
			g.handCount[i] = rand() % MAX_HAND;
			for(int j = 0; j < g.handCount[i]; j++) {
				g.hand[i][j] = rand() % (treasure_map + 1);
			}
		}

		// just in case we zero'd player 1's hand (they need to hold our target card)
		if(g.handCount[g.whoseTurn] == 0)
			g.handCount[g.whoseTurn] = 1;

		// plant the Council Room card within player 1's hand
		int crPosition = rand() % g.handCount[g.whoseTurn];
		g.hand[g.whoseTurn][crPosition] = council_room;

		// save relevant information about our "pre" state
		remember(&g);

		// play the Council Room card
		playCard(crPosition, 0, 0, 0, &g);

		// verify out "post" state
		verifyBehavior(&g);
	}

	return 0;
}