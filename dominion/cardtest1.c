#include <stdio.h>
#include "test-lib.h"
#include "dominion.h"

int main() {
	start("cardEffect (Cutpurse)");

	struct gameState g;

	int k[10] = {smithy,adventurer,gardens,embargo,cutpurse,mine,ambassador,
	       outpost,baron,tribute};

  	int r = initializeGame(4, k, 5, &g);
  	assert(r == 0, "Game initialized.");

  	// now we've got to mock some stuff...
  	g.whoseTurn = 1;

  	// set player 0 to have copper and nothing else
  	g.handCount[0] = 1;
  	g.hand[0][0] = copper;

  	// set player 1 to have a cutpurse and nothing else
  	g.handCount[1] = 1;
  	g.hand[1][0] = cutpurse;

  	// set player 2 to have copper and something else
  	g.handCount[2] = 2;
  	g.hand[2][0] = copper;
  	g.hand[2][1] = 99;

  	// set player 3 to have nothing of interest
  	g.handCount[3] = 1;
  	g.hand[3][0] = 99;

  	g.coins = 0;

  	playCard(0, -1, -1, -1, &g);

  	// test 1: ensure we were given our precious bonus coins
  	assert(g.coins == 2, "Coins incremented by 2.");

  	// test 2: ensure copper was removed from player 0
  	assert(g.handCount[0] == 0, "Copper is removed from another player's hand. (Player had only copper)");

  	// test 3: ensure copper was removed from player 2
  	assert(g.handCount[2] == 1 && g.hand[2][0] == 99, "Copper is removed from another player's hand. (Player had copper + another card)");

  	// test 4: ensure player 3 was untouched
  	assert(g.handCount[3] == 1 && g.hand[3][0] == 99, "A player without copper remains untouched.");

	end();
}