#include <stdio.h>
#include "test-lib.h"
#include "dominion.h"

int main() {
	start("cardEffect (Sea Hag)");

	struct gameState g;

	int k[10] = {smithy,adventurer,gardens,embargo,cutpurse,mine,ambassador,
	       outpost,baron,sea_hag};

  	int r = initializeGame(4, k, 5, &g);
  	assert(r == 0, "Game initialized.");

  	// now we've got to mock some stuff...
  	g.whoseTurn = 0;

  	// set player 0 to have Sea Hag and nothing else
  	g.handCount[0] = 1;
  	g.hand[0][0] = sea_hag;

  	// set player 0's deck to contain 1 arbitrary card
  	g.deck[0][0] = 99;
  	g.deckCount[0] = 1;

  	// set player 1's deck to contain one arbitrary card
  	g.deck[1][0] = 99;
  	g.deckCount[1] = 1;
  	g.discardCount[1] = 0;

  	// set player 2's deck to contain two (distinct) arbitrary cards
  	g.deckCount[2] = 2;
  	g.deck[2][0] = 99;
  	g.deck[2][1] = 100;
  	g.discardCount[2] = 0;

  	// set player 3's deck to contain no cards
  	g.deckCount[3] = 0;

  	playCard(0, -1, -1, -1, &g);

  	// test 1: ensure player wasn't affected
  	assert(g.deckCount[0] == 1 && g.deck[0][0] == 99, "Player's deck remains untouched.");

  	// test 2: ensure all other player's top deck card is discarded
  	assert(g.discard[1][g.discardCount[1] - 1] == 99, "Another player's top deck card was discarded. (One card in deck)");

  	assert(g.discard[2][g.discardCount[2] - 1] == 100, "Another player's top deck card was discarded. (Two cards in deck)");

  	// test 3: ensure 0 deck count is handled
  	assert(g.deckCount[3] > 0, "Another player's discards were shuffled to form deck. (No cards in deck)");

  	// test 4: ensure curse was added to deck
  	assert(g.deck[1][g.deckCount[1] - 1] == curse, "Another player's top deck card is now curse. (One card in deck)");

  	assert(g.deck[2][g.deckCount[2] - 1] == curse, "Another player's top deck card is now curse. (Two cards in deck)");

	end();
}