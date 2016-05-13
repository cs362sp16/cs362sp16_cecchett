#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "softassert.h"

#define TEST_COUNT 100000

int treasureInHand;
int treasureShouldBeAdded;
int totalCards;

int countTreasure(int cards[], int size) {
	int treasure = 0;
	for(int i = 0; i < size; i++) {
		if(cards[i] == copper || cards[i] == silver || cards[i] == gold)
			treasure++;
	}
	return treasure;
}

void remember(struct gameState* g) {
	// remember how much treasure we have in our hand
	treasureInHand = countTreasure(g->hand[g->whoseTurn], g->handCount[g->whoseTurn]);

	// determine how much should be added (max 2 from deck, less if it contains less than 2)
	int treasureInDeck = countTreasure(g->deck[g->whoseTurn], g->deckCount[g->whoseTurn]);
	treasureShouldBeAdded = treasureInDeck >= 2 ? 2 : treasureInDeck;

	// remember how many cards we have in total
	totalCards = g->discardCount[g->whoseTurn] + g->handCount[g->whoseTurn] + g->deckCount[g->whoseTurn];
}

void verifyBehavior(struct gameState* g) {
	// make sure appropriate amount of treasure was added to our hand
	softassert(treasureInHand + treasureShouldBeAdded == countTreasure(g->hand[g->whoseTurn], g->handCount[g->whoseTurn]), "We didn't gain 2 treasure cards.");
	
	// make sure we didn't lose/gain cards overall
	softassert(totalCards == g->discardCount[g->whoseTurn] + g->handCount[g->whoseTurn] + g->deckCount[g->whoseTurn], "We gained/lost cards.");
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

		// set all hands and decks to random cards
		for(int i = 0; i < g.numPlayers; i++) {
			g.handCount[i] = rand() % MAX_HAND;
			for(int j = 0; j < g.handCount[i]; j++) {
				g.hand[i][j] = rand() % (treasure_map + 1);
			}

			g.deckCount[i] = rand() % MAX_DECK;
			for(int j = 0; j < g.deckCount[i]; j++) {
				g.deck[i][j] = rand() % (treasure_map + 1);
			}
		}

		// just in case we zero'd player 1's hand (they need to hold our target card)
		if(g.handCount[g.whoseTurn] == 0)
			g.handCount[g.whoseTurn] = 1;

		// plant the Adventurer card within player 1's hand
		int adventurerPosition = rand() % g.handCount[g.whoseTurn];
		g.hand[g.whoseTurn][adventurerPosition] = adventurer;

		// save relevant information about our "pre" state
		remember(&g);

		// play the Council Room card
		playCard(adventurerPosition, 0, 0, 0, &g);

		// verify out "post" state
		verifyBehavior(&g);
	}

	return 0;
}