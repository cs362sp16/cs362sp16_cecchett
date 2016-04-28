#include <stdio.h>
#include "test-lib.h"
#include "dominion.h"

int main() {
	start("cardEffect (Smithy)");

	struct gameState g;

	int k[10] = {smithy,adventurer,gardens,embargo,cutpurse,mine,ambassador,
	       outpost,baron,sea_hag};

  	int r = initializeGame(4, k, 5, &g);
  	assert(r == 0, "Game initialized.");

  	// let's mock some stuff
  	g.whoseTurn = 0;
  	g.hand[0][0] = smithy;

  	int handCountBefore = g.handCount[0];

  	playCard(0, -1, -1, -1, &g);

  	assert(g.handCount[0] = handCountBefore + 2, "Player draws three cards (hand count += 2, since Smithy is removed).");
  	assert(g.hand[0][0] != smithy, "Player's smithy card is removed.");

	end();
}