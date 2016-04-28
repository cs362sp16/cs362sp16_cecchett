#include <stdio.h>
#include "test-lib.h"
#include "dominion.h"

int main() {
	start("cardEffect (Council Room)");

	struct gameState g;

	int k[10] = {smithy,adventurer,gardens,embargo,cutpurse,mine,ambassador,
	       outpost,baron,council_room};

  	int r = initializeGame(4, k, 5, &g);
  	assert(r == 0, "Game initialized.");

  	// mock the scenario
  	g.whoseTurn = 0;
  	g.hand[0][0] = council_room;
  	g.handCount[0] = 1;

  	g.handCount[1] = 0;
  	g.handCount[2] = 1;

  	int numBuysBefore = g.numBuys;

  	playCard(0, -1, -1, -1, &g);

  	assert(g.numBuys == numBuysBefore + 1, "Number of buys is increased by 1.");
  	assert(g.handCount[0] == 4, "Player draws 4 cards.");
  	assert(g.handCount[1] == 1, "Another player draws 1 card. (No cards in hand)");
  	assert(g.handCount[2] == 2, "Another player draws 1 card. (One card in hand)");

	end();
}