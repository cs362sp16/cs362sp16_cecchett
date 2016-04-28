#include <stdio.h>
#include "test-lib.h"
#include "dominion.h"

int main() {
	start("handCard");

	struct gameState g;

	// stub current player & card in pos 4
	g.whoseTurn = 1;
	g.hand[g.whoseTurn][4] = 6;

	assert(handCard(4, &g) == 6, "Returns correct value in a good state. ");

	// stub bad/invalid state (current player out of bounds)
	g.whoseTurn = -1;

	assert(handCard(4, &g) == -1, "Detects invalid game state. (Current player out of bounds)");

	// stub valid state, but pass invalid card position
	g.whoseTurn = 0;

	assert(handCard(-1, &g) == -1, "Detects invalid card index. (Card position out of bounds)");

	end();
}