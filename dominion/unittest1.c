#include <stdio.h>
#include "test-lib.h"
#include "dominion.h"

int main() {
	start("whoseTurn");

	struct gameState g;

	g.whoseTurn = 1;
	assert(whoseTurn(&g) == 1, "Returns correct value in a good state. ");

	g.whoseTurn = 7;
	assert(whoseTurn(&g) != 7, "Identifies when the value has become corrupt/invalid. (Too large)");

	g.whoseTurn = -5;
	assert(whoseTurn(&g) != -5, "Identifies when the value has become corrupt/invalid. (Too small)");

	end();
}