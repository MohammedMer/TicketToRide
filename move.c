#include "move.h"

/* plays the move given as a parameter (send to the serveur)
 * returns the return code */
void moveChoice(t_move* move){

	/* move type choice */
	printf("Choose a move\n");
	printf("\t1 to claim a route\n");
	printf("\t2 to draw a blind card\n");
	printf("\t3 to draw a card\n");
	printf("\t4 to draw objectives\n");
	printf("\t5 to choose objectives\n");

	scanf("%d", (int*) &move->type);

	/* filling of parameters */
	switch(move->type){
		case CLAIM_ROUTE:
			printf("Enter city1, city2, color and number of locomotives: ");
			scanf("%d %d %d %d", &move->claimRoute.city1, &move->claimRoute.city2, (int*) &move->claimRoute.color, &move->claimRoute.nbLocomotives);
			break;

		case DRAW_CARD:
			printf("Give the color: ");
			scanf("%d", (int*) &move->drawCard.card);
			break;

		case CHOOSE_OBJECTIVES:
			printf("For each objective, give 0 or 1:\n\t");
			scanf("%d %d %d", &move->chooseObjectives.chosen[0], &move->chooseObjectives.chosen[1], &move->chooseObjectives.chosen[2]);
			break;

		default:
			break;
	}
	printf("\n");
}
 

/* plays the move given as a parameter (send to the server)
 * returns the return code */
t_return_code playOurMove(t_move* move, t_color* lastCard, t_objective obj[3]){
	t_return_code toReturn;

	switch (move->type) {
		case CLAIM_ROUTE:
			toReturn = claimRoute(move->claimRoute.city1, move->claimRoute.city2, (int) move->claimRoute.color, move->claimRoute.nbLocomotives);
			*lastCard = NONE;
			break;

		case DRAW_CARD:
			toReturn = drawCard(move->drawCard.card, move->drawCard.faceUp);
			*lastCard = (*lastCard==NONE && move->drawCard.card!= MULTICOLOR) ? move->drawCard.card : NONE;
			break;

		case DRAW_BLIND_CARD:
			toReturn = drawBlindCard(&move->drawBlindCard.card);
			*lastCard = (*lastCard==NONE) ? move->drawBlindCard.card : NONE;
			break;

		case DRAW_OBJECTIVES:
			toReturn = drawObjectives(move->drawObjectives.objectives);
			printf("Make sure to take at least 2 objectives !\n");
			for(int i=0; i<3; i++){
				printf("\t %d. %d (", i+1, move->drawObjectives.objectives[i].city1);
				printCity(move->drawObjectives.objectives[i].city1);
				printf(") -> (");
				printCity(move->drawObjectives.objectives[i].city2);
				printf(") %d (%d pts)\n", move->drawObjectives.objectives[i].city2, move->drawObjectives.objectives[i].score);
			}

			for (int i=0; i<3; i++)
				obj[i] = move->drawObjectives.objectives[i];

			*lastCard = NONE;
			break;

		case CHOOSE_OBJECTIVES:
			move->chooseObjectives.nbObjectives = move->chooseObjectives.chosen[0] + move->chooseObjectives.chosen[1] + move->chooseObjectives.chosen[2];
			toReturn = chooseObjectives(move->chooseObjectives.chosen);
			*lastCard = NONE;
			break;
	}
	printf("\n");

	return toReturn;
}

/* tell if we need to replay */
int needReplay(t_move* move, t_color lastCard){
	int replay = 0;

	switch(move->type){
		case DRAW_OBJECTIVES:
			replay = 1;
			break;

		case DRAW_BLIND_CARD:
			if (lastCard == NONE)
				replay = 1;
			break;

		case DRAW_CARD:
			if (lastCard == NONE && move->drawCard.card != MULTICOLOR)
				replay = 1;
			break;

		case CLAIM_ROUTE:
			replay = 0;
			break;

		default:
			break;
	}
	return replay;
}