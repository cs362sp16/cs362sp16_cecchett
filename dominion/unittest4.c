#include <stdio.h>
#include "test-lib.h"
#include "dominion.h"

int updateCoins(int player, struct gameState *state, int bonus);

int main() {
	start("updateCoins");

	struct gameState g;

	// initialize game with arbitrary deck/hand/discard pile sizes and invalid cards in all spots

	int stubPlayer = 1;

	g.whoseTurn = stubPlayer;

	g.deckCount[stubPlayer] = 41;
	g.handCount[stubPlayer] = 7;
	g.discardCount[stubPlayer] = 32;

	// set all deck values to invalid card
	for(int i = 0; i < g.deckCount[stubPlayer]; i++) {
		g.deck[stubPlayer][i] = 99;
	}

	// set all hand values to invalid card
	for(int i = 0; i < g.handCount[stubPlayer]; i++) {
		g.hand[stubPlayer][i] = 99;
	}

	// set all discard values to invalid card
	for(int i = 0; i < g.discardCount[stubPlayer]; i++) {
		g.discard[stubPlayer][i] = 99;
	}

	// test 1: cards of all types are counted

	g.hand[stubPlayer][1] = copper;
	updateCoins(stubPlayer, &g, 0);
	assert(g.coins == 1, "Copper coins are counted.");

	g.hand[stubPlayer][2] = silver;
	updateCoins(stubPlayer, &g, 0);
	assert(g.coins == 3, "Silver coins are counted.");

	g.hand[stubPlayer][3] = gold;
	updateCoins(stubPlayer, &g, 0);
	assert(g.coins == 6, "Gold coins are counted.");

	// test 2: make sure only cards within hand are counted

	// add to discard and deck piles
	g.discard[stubPlayer][3] = silver;
	g.deck[stubPlayer][3] = gold;

	updateCoins(stubPlayer, &g, 0);
	assert(g.coins == 6, "Only cards within hand are counted.");

	// test 3: make sure cards for only given player are counted

	g.hand[stubPlayer + 1][1] = silver;

	updateCoins(stubPlayer, &g, 0);
	assert(g.coins == 6, "Only cards for given player are counted.");

	end();
}