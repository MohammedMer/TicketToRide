#include <stdio.h>
#include <stdlib.h>

#include "TicketToRideAPI.h"
#include "game.h"
#include "data_game.h"
#include "move.h"

int main(){

	char* serverName = "li1417-56.members.linode.com";
	unsigned int port = 5678;
	char gameType[100] = "TRAINING DO_NOTHING timeout=1000 start=0 map=USA";

	t_return_code movePlayed = NORMAL_MOVE; /* save retCode */
	int replay = 0;			/* boolean, tells if the player replays */
	t_move move;			/* current move */
	t_color lastMove = NONE;	/* last Card taken we need to replay else NONE */
	t_color ourCards[4];	/* cards owned */
	t_objective obj[3];

	/* variable containing all game's parameters */
	t_game game;

	/* game's creation */
	initGame(&game, serverName, port, gameType);

	/* create map and take game's informations */
	initMap(&game, ourCards);

	/* take player's informations */
	players(&game, ourCards);

	/* print informations of the curent game */
	currentGame(&game);

	int turn = 1;	/* turn's number */

	while (movePlayed == NORMAL_MOVE){
		printf("Turn n° %d:\n", turn);

		/* when we play */
		if (game.curentPlayer == 0){
			printMap();
			printf("I play... \n");
			currentGame(&game);

			moveChoice(&move);
			replay = needReplay(&move, lastMove);
			movePlayed = playOurMove(&move, &lastMove, obj);
			updateGame(&game, &move, obj);

			if((replay) && (movePlayed == NORMAL_MOVE)){
				//printMap();
				printf("...I play again...\n");
				currentGame(&game);

				moveChoice(&move);
				replay = needReplay(&move, lastMove);
				movePlayed = playOurMove(&move, &lastMove, obj);
				updateGame(&game, &move, obj);
			}
			printf("...I am done\n");
			
		}
		else{
			/* when the opponent play */
			if(movePlayed == NORMAL_MOVE){
				printf("The opponent play: loading...\n");
				currentGame(&game);

				movePlayed = getMove(&move,&replay);
				updateGame(&game, &move, obj);
			}

			while((replay) && (movePlayed == NORMAL_MOVE)){
				printf("...the opponent play again...\n");
				currentGame(&game);

				movePlayed = getMove(&move,&replay);
				updateGame(&game, &move, obj);
			}
			printf("...task achived.\n");
		}

		/* change player */
		if (movePlayed == NORMAL_MOVE && !replay){
			game.curentPlayer = !game.curentPlayer;
			turn++;
		}

	}
	printf("Game finished.\n");

	if ((movePlayed == WINNING_MOVE && movePlayed == 0) || (movePlayed == LOOSING_MOVE && game.playerId == 1)){
		printf("We learn more when you lose than when you win 🙃\n");
	}
	else{
		printf("The opponent has exploited a vulnerability 👀️\n");
	}

	printf("Disconnection of the server...\n");
	closeConnection();

	return 0;
}