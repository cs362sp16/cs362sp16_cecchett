#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"

int getCost(int card);

void randomCards(int* deck) {
	for(int i = 0; i < 10; i++) {
		int randCard = rand() % (treasure_map + 1);

		int deckAlreadyContains = 0;
		for(int j = 0; j < i; j++) {
			if(deck[j] == randCard) {
				deckAlreadyContains = 1;
				break;
			}
		}
		if(deckAlreadyContains) {
			i--;
			continue;
		}

		deck[i] = randCard;
	}
}

int randomAffordableCard(struct gameState *g) {
	int affordableCards[treasure_map + 1];
	int affordableCardsCount = 0;

	for(int i = 0; i <= treasure_map; i++) {
		if(g->supplyCount[i] > 0 && getCost(i) <= g->coins) {
			affordableCards[affordableCardsCount++] = i;
		}
	}

	if(affordableCardsCount == 0) {
		return -1;
	}

	int randCard = rand() % affordableCardsCount;
	return affordableCards[randCard];
}

int randomActionCard(struct gameState *g, int* ignore, int ignorec) {
	int playableCards[numHandCards(g)];
	int playableCardsCount = 0;

	for(int i = 0; i < numHandCards(g); i++) {
		if(handCard(i, g) >= adventurer) {
			int ignoreCard = 0;
			for(int j = 0; j < ignorec; j++)
				if(ignore[j] == i)
					ignoreCard = 1;

			if(!ignoreCard) {
				printf("Card %d is playable, it's %d\n", i, handCard(i, g));
				playableCards[playableCardsCount++] = i;
			}
		}
	}

	if(playableCardsCount == 0) {
		return -1;
	}

	int randCard = rand() % playableCardsCount;
	return playableCards[randCard];
}

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

	int seed = atoi(argv[1]);
	srand(seed);

	for(int i = 0; i < 20; i++) {
		int kingdomCards[10];
		randomCards(kingdomCards);

		int randPlayers = rand() % 3 + 2;

		struct gameState g;
		initializeGame(randPlayers, kingdomCards, rand(), &g);

		while(!isGameOver(&g)) {
			printf("Game isn't over\n");

			int error[MAX_HAND];
			int errorCount = 0;
			while(g.numActions > 0 && numHandCards(&g) > 0) {
				int card = randomActionCard(&g, error, errorCount);
				if(card == -1)
					break;

				int choices[3];
				if(randomChoices(card, choices, &g)) {
					error[errorCount++] = card;
					continue;
				}

				if(playCard(card, choices[0], choices[1], choices[2], &g) > -1) {
					printf("Playing action card %d\n", handCard(card, &g));
					errorCount = 0;
				} else {
					error[errorCount++] = card;
					printf("Failed to play card at index %d (value: %d), ignoring.\n", card, handCard(card, &g));
				}
			}

			for(int i = 0; i < numHandCards(&g); i++) {
				if((handCard(i, &g) == copper || handCard(i, &g) == silver 
					|| handCard(i, &g) == gold) && rand() % 3 < 2) {
					printf("Playing money card\n");
					playCard(i, -1, -1, -1, &g);
				}
			}

			while(g.numBuys > 0) {
				int card = randomAffordableCard(&g);
				if(card == -1)
					break;
				printf("Buying something\n");
				buyCard(card, &g);
			}

			printf("Ending turn\n");
			endTurn(&g);
		}
		printf("Game is over\n");

		int winners[MAX_PLAYERS];
		getWinners(winners, &g);

		printf("Results:\n");
		for(int i = 0; i < g.numPlayers; i++) {
			printf("  Score for player %d: %d. Winner? %d\n", i + 1, scoreFor(i, &g), winners[i]);
		}
	}
	

}
