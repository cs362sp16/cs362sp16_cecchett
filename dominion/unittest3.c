#include <stdio.h>
#include "test-lib.h"
#include "dominion.h"

int main() {
	start("fullDeckCount");

	// test 1: we set all cards within deck/hand/discard to a certain card value, then ask for its count

	struct gameState g;

	int stubCard = 99;
	int stubPlayer = 1;
	
	// stub all these values
	g.deckCount[stubPlayer] = 41;
	g.handCount[stubPlayer] = 7;
	g.discardCount[stubPlayer] = 32;

	// stub all deck values
	for(int i = 0; i < g.deckCount[stubPlayer]; i++) {
		g.deck[stubPlayer][i] = stubCard;
	}

	// stub all hand values
	for(int i = 0; i < g.handCount[stubPlayer]; i++) {
		g.hand[stubPlayer][i] = stubCard;
	}

	// stub all discard values
	for(int i = 0; i < g.discardCount[stubPlayer]; i++) {
		g.discard[stubPlayer][i] = stubCard;
	}

	assert(fullDeckCount(stubPlayer, stubCard, &g) == g.deckCount[stubPlayer] + g.handCount[stubPlayer] + g.discardCount[stubPlayer], "All cards are checked within deck/hand/discard.");

	// test 2: we set only some cards within deck/hand/discard to a certain card value, then ask for its count

	// all our cards are currently equal to stubCard, let's change that
	g.deck[stubPlayer][3] = 0;
	g.hand[stubPlayer][5] = 0;
	g.discard[stubPlayer][1] = 0;

	// we changed 3 cards, thus it should be 3 less than the total amount of cards
	assert(fullDeckCount(stubPlayer, stubCard, &g) == g.deckCount[stubPlayer] + g.handCount[stubPlayer] + g.discardCount[stubPlayer] - 3, "Counts only the desired card.");

	end();
}