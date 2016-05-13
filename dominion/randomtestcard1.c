#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "softassert.h"

#define TEST_COUNT 100000

int coins;
int copperCount[MAX_PLAYERS];
int handCount[MAX_PLAYERS];

void remember(struct gameState* g) {
	coins = g->coins;
	for(int i = 0; i < g->numPlayers; i++) {
		copperCount[i] = 0;
		handCount[i] = g->handCount[i];
		if(i == g->whoseTurn)
			continue;
		for(int j = 0; j < g->handCount[i]; j++) {
			if(g->hand[i][j] == copper) {
				copperCount[i]++;
			}
		}
	}
}

void verifyBehavior(struct gameState* g) {
	softassert(g->coins == coins + 2, "Coins wasn't incrememnted by 2.");

	int copperCountPost[MAX_PLAYERS];
	for(int i = 0; i < g->numPlayers; i++) {
		copperCountPost[i] = 0;
		if(i == g->whoseTurn)
			continue;
		for(int j = 0; j < g->handCount[i]; j++) {
			if(g->hand[i][j] == copper) {
				copperCountPost[i]++;
			}
		}

		if(copperCount[i] > 0) {
			softassert(copperCountPost[i] == copperCount[i] - 1, "A player was not deducted a copper.");
		}

		softassert(copperCount[i] == 0 ? (g->handCount[i] == handCount[i]) : (g->handCount[i] == handCount[i] - 1), "Hand count was not correct.");
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

		// plant the Cutpurse card within player 1's hand
		int cutpursePosition = rand() % g.handCount[g.whoseTurn];
		g.hand[g.whoseTurn][cutpursePosition] = cutpurse;

		// update coins to reflect new random deck
		updateCoins(g.whoseTurn, &g, 0);

		// save relevant information about our "pre" state
		remember(&g);

		// play the Cutpurse card
		playCard(cutpursePosition, 0, 0, 0, &g);

		// verify out "post" state
		verifyBehavior(&g);
	}

	return 0;
}