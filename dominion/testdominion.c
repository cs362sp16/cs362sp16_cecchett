#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"

// Define function not listed in dominion.h to suppress compiler warnings
int getCost(int card);

/**
 * Fills a deck with a random set of 10 unique cards (may not all be valid)
 */
void randomCards(int* deck) {
	for(int i = 0; i < 10; i++) {
		int randCard = rand() % (treasure_map + 1);

		// Make sure card is unique
		int deckAlreadyContains = 0;
		for(int j = 0; j < i; j++) {
			if(deck[j] == randCard) {
				deckAlreadyContains = 1;
				break;
			}
		}

		//If not, try again
		if(deckAlreadyContains) {
			i--;
			continue;
		}

		deck[i] = randCard;
	}
}

/**
 * Finds a random card that the player is able to purchase (supply > 0 and coins >= cost)
 * @return the index in the supply array (i.e. the card's value)
 */
int randomAffordableCard(struct gameState *g) {
	int affordableCards[treasure_map + 1];
	int affordableCardsCount = 0;

	for(int i = 0; i <= treasure_map; i++) {
		// If card is affordable and supply > 0, add it to array
		if(g->supplyCount[i] > 0 && getCost(i) <= g->coins) {
			affordableCards[affordableCardsCount++] = i;
		}
	}

	// Couldn't find any cards to buy
	if(affordableCardsCount == 0) {
		return -1;
	}

	int randCard = rand() % affordableCardsCount;
	return affordableCards[randCard];
}

/**
 * Finds a random action card in the player's hand. Ignores all cards whose index in
 * the player's hand is contained within the ignore array.
 * @return the index in the player's hand of an action card
 */
int randomActionCard(struct gameState *g, int* ignore, int ignorec) {
	int playableCards[numHandCards(g)];
	int playableCardsCount = 0;

	for(int i = 0; i < numHandCards(g); i++) {
		if(handCard(i, g) >= adventurer) {
			// Check if we're supposed to ignore this one
			int ignoreCard = 0;
			for(int j = 0; j < ignorec; j++)
				if(ignore[j] == i)
					ignoreCard = 1;

			// If not, add it to playable cards array
			if(!ignoreCard) {
				printf("Card %d is playable, it's %d\n", i, handCard(i, g));
				playableCards[playableCardsCount++] = i;
			}
		}
	}

	// Couldn't find a card to play
	if(playableCardsCount == 0) {
		return -1;
	}

	int randCard = rand() % playableCardsCount;
	return playableCards[randCard];
}

/**
 * Given that the player is about to play card at cardIndex, this function
 * will attempt to fill the choices array with 3 valid choices.
 * @return 0 if successful, -1 if valid choices are not found
 */
int randomChoices(int cardIndex, int *choices, struct gameState *g) {
	int card = handCard(cardIndex, g);
	switch(card) {
		case feast: /* choice1 is supply # of card gained) */
		{
			int possibleCards[treasure_map + 1];
			int possibleCardsCount = 0;
			for(int i = 0; i <= treasure_map; i++) {
				if(getCost(i) <= 5 && g->supplyCount[i] > 0) {
					possibleCards[possibleCardsCount++] = i;
				}
			}

			if(possibleCardsCount == 0)
				return -1;

			choices[0] = possibleCards[rand() % possibleCardsCount];
			choices[1] = -1;
			choices[2] = -1;
			break;
		}

		case mine: /* choice1 is hand# of money to trash, choice2 is supply# of
	    money to put in hand */
	    {
	    	int possibleCards[numHandCards(g)];
			int possibleCardsCount = 0;
	    	for(int i = 0; i < numHandCards(g); i++) {
	    		if(handCard(i, g) < copper || handCard(i, g) > gold)
	    			continue;
	    		possibleCards[possibleCardsCount++] = i;
	    	}

	    	if(possibleCardsCount == 0)
	    		return -1;

	    	int toTrash = possibleCards[rand() % possibleCardsCount];
	    	int trashCardValue = getCost(handCard(toTrash, g));

	    	int treasure[3] = {copper, silver, gold};
	    	int possibleToAdd[3];
	    	int possibleToAddCount = 0;
	    	for(int i = 0; i < 3; i++) {
	    		if(g->supplyCount[treasure[i]] > 0 && getCost(treasure[i]) <= trashCardValue + 3)
	    			possibleToAdd[possibleToAddCount++] = i;
	    	}

	    	if(possibleToAddCount == 0)
	    		return -1;

	    	int toAdd = possibleToAdd[rand() % possibleToAddCount];

	    	choices[0] = toTrash;
	    	choices[1] = toAdd;
	    	choices[2] = -1;

	    	break;
	    }

		case remodel: /* choice1 is hand# of card to remodel, choice2 is supply# */
	    {
	    	int randomHandPositions[numHandCards(g)];
			int randomHandPositionsCount = 0;
			for(int i = 0; i < numHandCards(g); i++) {
				if(i != cardIndex)
					randomHandPositions[randomHandPositionsCount++] = i;
			}

			if(randomHandPositionsCount == 0)
				return -1;

			int handPos = randomHandPositions[rand() % randomHandPositionsCount];
			int cardCost = getCost(handCard(handPos, g));

			int randomSupplyCards[treasure_map + 1];
			int randomSupplyCardCount = 0;
			for(int i = 0; i <= treasure_map; i++) {
				if(g->supplyCount[i] > 0 && getCost(i) <= cardCost + 2)
					randomSupplyCards[randomSupplyCardCount++] = i;
			}

			if(randomSupplyCardCount == 0)
				return -1;

			int newCard = randomSupplyCards[rand() % randomSupplyCardCount];

	    	choices[0] = handPos;
	    	choices[1] = newCard;
	    	choices[2] = -1;
	    	break;
	    }

		case baron: /* choice1: boolean for discard of estate */
	    {
	    	int haveEstate = 0;
	    	for(int i = 0; i < numHandCards(g); i++) {
	    		if(handCard(i, g) == estate) {
	    			haveEstate = 1;
	    			break;
	    		}
	    	}

	    	choices[0] = haveEstate ? rand() % 2 : 0;
	    	choices[1] = -1;
	    	choices[2] = -1;
			break;
	    }

		case minion: /* choice1:  1 = +2 coin, 2 = redraw */
		{
			int which = rand() % 2;
			int other = which == 0 ? 1 : 0;
	    	choices[which] = 1;
	    	choices[other] = 0;
	    	choices[2] = -1;
			break;
		}

		case steward: /* choice1: 1 = +2 card, 2 = +2 coin, 3 = trash 2 (choice2,3) */
		{
			choices[0] = rand() % 3 + 1;
			choices[1] = -1;
			choices[2] = -1;

			if(choices[0] == 3) {
				if(numHandCards(g) < 3) {
					choices[0] = rand() % 2 + 1;
					break;
				}

				int randomHandPositions[2];
				int randomHandPositionsCount = 0;
				while(randomHandPositionsCount < 2) {
					int i = rand() % numHandCards(g);
					if(i == cardIndex)
						continue;
					randomHandPositions[randomHandPositionsCount++] = i;
				}

				choices[1] = randomHandPositions[0];
				choices[2] = randomHandPositions[1];
			}
			break;
		}

		case ambassador: /* choice1 = hand#, choice2 = number to return to supply */
		{
			int randomHandPositions[numHandCards(g)];
			int randomHandPositionsCount = 0;
			for(int i = 0; i < numHandCards(g); i++) {
				if(i != cardIndex)
					randomHandPositions[randomHandPositionsCount++] = i;
			}

			if(randomHandPositionsCount == 0)
				return -1;

			int handPos = randomHandPositions[rand() % randomHandPositionsCount];

			int cardCount = 0;
			for(int i = 0; i < numHandCards(g); i++) {
				if(handCard(i, g) == handCard(handPos, g))
					cardCount++;
			}

			choices[0] = handPos;
			choices[1] = cardCount > 1 ? (rand() % 2 + 1) : 1;
			choices[2] = -1;

			break;
		}

		case embargo: /* choice1 = supply# */
		{
			choices[0] = rand() % (treasure_map + 1);
			choices[1] = -1;
			choices[2] = -1;
			break;
		}

		case salvager: /* choice1 = hand# to trash */
		{
			int randomHandPositions[numHandCards(g)];
			int randomHandPositionsCount = 0;
			for(int i = 0; i < numHandCards(g); i++) {
				if(i != cardIndex)
					randomHandPositions[randomHandPositionsCount++] = i;
			}

			if(randomHandPositionsCount == 0)
				return -1;

			choices[0] = randomHandPositions[rand() % randomHandPositionsCount];
			choices[1] = -1;
			choices[2] = -1;
			break;
		}

		default:
			choices[0] = -1;
			choices[1] = -1;
			choices[2] = -1;
			break;
	}

	return 0;
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Syntax: ./testdominion <random seed>\n");
		return -1;
	}

	// Set random seed from arguments
	int seed = atoi(argv[1]);
	srand(seed);

	for(int i = 0; i < 20; i++) {
		// Get random kingdom cards
		int kingdomCards[10];
		randomCards(kingdomCards);

		// Set random amount of players 2-4
		int randPlayers = rand() % 3 + 2;

		// Initialize game
		struct gameState g;
		initializeGame(randPlayers, kingdomCards, rand(), &g);

		// Play out the game
		while(!isGameOver(&g)) {
			printf("test: Game isn't over\n");

			// Hold array of cards we failed to play
			int error[MAX_HAND];
			int errorCount = 0;

			// Possible to play a card
			while(g.numActions > 0 && numHandCards(&g) > 0) {
				int card = randomActionCard(&g, error, errorCount);

				// No playable card found
				if(card == -1)
					break;

				// Get random choices for the card. If none are found (not able to play card), continue
				int choices[3];
				if(randomChoices(card, choices, &g)) {
					error[errorCount++] = card;
					continue;
				}

				// Play the card, see if we were successful
				if(playCard(card, choices[0], choices[1], choices[2], &g) > -1) {
					printf("test: Playing action card %d\n", handCard(card, &g));
					errorCount = 0;
				} else {
					error[errorCount++] = card;
					printf("test: Failed to play card at index %d (value: %d), ignoring.\n", card, handCard(card, &g));
				}
			}

			// Throw out random treasure cards -- 2/3 chance for each in hand (arbitrary)
			for(int i = 0; i < numHandCards(&g); i++) {
				if((handCard(i, &g) == copper || handCard(i, &g) == silver || handCard(i, &g) == gold) && rand() % 3 < 2) {
					printf("test: Playing treasure card\n");
					playCard(i, -1, -1, -1, &g);
				}
			}

			// Buy random cards while we can
			while(g.numBuys > 0) {
				int card = randomAffordableCard(&g);
				if(card == -1)
					break;
				printf("test: Buying something\n");
				buyCard(card, &g);
			}

			printf("test: Ending turn\n");
			endTurn(&g);
		}
		printf("test: Game is over\n");

		int winners[MAX_PLAYERS];
		getWinners(winners, &g);

		printf("Results:\n");
		for(int i = 0; i < g.numPlayers; i++) {
			printf("  test: Score for player %d: %d. Winner? %d\n", i + 1, scoreFor(i, &g), winners[i]);
		}
	}
	

}
