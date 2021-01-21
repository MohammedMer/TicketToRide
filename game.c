#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "TicketToRideAPI.h"
#include "game.h"
#include "data_game.h"
#include "move.h"
#include "algo.h"

#define N  NB_MAX_CITIES

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
	int turn = 1;	/* turn's number */

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

	
	/* Graph initialization */
	int G[N][N];
	for(int i=0; i < N; i++){
		for(int j=0; j < N; j++){
			
			if (game.gameboard.tracks[i][j].lengthTrack != 0){
				G[i][j] = game.gameboard.tracks[i][j].lengthTrack;
			}
			else{
				G[i][j]	= INT_MAX;
			}
		}
	}

	/* array in wich D[i] represents the shortest distance between the cities src and i */
	int* D = (int*) malloc(N*sizeof(int));
	/* array in wich Prev[i] represents the previous city from the source */
	int* Prev = (int*) malloc(N*sizeof(int));

	int nbObjectivesListed = 0;

	int* ShortestTrack = (int*) malloc(20*N*sizeof(int));
	/* this array contains the number of cities of the shorter Track for each objectives */
	int* lengthShortestTrackObjectives = (int*) malloc(20*sizeof(int)); 
	int nbCitiesShortestTrackObjectives = 0;
	int nbElemShortestTrack = 0;


	/*variables created to chose a face up card*/
	int* desiredColor = (int*) malloc(10*sizeof(int));
	int* priorityOrder = (int*) malloc(10*sizeof(int)); /*array containing colors sorted by by priority (number of the card color we need) */
	int all; /* counter from 0 to 10  */

	while (movePlayed == NORMAL_MOVE){
		printf("Turn n° %d:\n", turn);

		/* when we play */
		if (game.curentPlayer == 0){
			printMap();
			printf("I play... \n");
			currentGame(&game);

			if(turn == 1){
//printf("Point 1:\n");				
				move.type = DRAW_OBJECTIVES;
			}
			else if (game.players[0].nbObjectives == 0){
//printf("Point 2:\n");
				move.chooseObjectives.chosen[ nbObjectivesListed ] = 1;
				
				move.type = CHOOSE_OBJECTIVES;
				chooseObjectives(move.chooseObjectives.chosen);
			}
			else {
//printf("Point 3:\n");				
				int city1;
				int city2;			
				int claimedColor_1;
				int claimedColor_2;
				int nbCardsColor_1;
				int nbCardsColor_2;
				int lengthTrackClaimed;

				int i=0,j=0;
				while ((i < game.players[0].nbObjectives) && (nbCardsColor_1 < lengthTrackClaimed) && (nbCardsColor_2 < lengthTrackClaimed)){
					while ((j < lengthShortestTrackObjectives[i] - 1) && (nbCardsColor_1 < lengthTrackClaimed) && (nbCardsColor_2 < lengthTrackClaimed)){

						city1 = ShortestTrack[i*N+j];
						city2 = ShortestTrack[i*N+j+1];
					printf(" %d (", city1);
					printCity(city1);
					printf(") -> (");
					printCity(city2);
					printf(") %d \n", city2);

						lengthTrackClaimed = game.gameboard.tracks[ city1 ][ city2 ].lengthTrack;

						claimedColor_1 = game.gameboard.tracks[ city1 ][ city2 ].colorTrack_1;
						claimedColor_2 = game.gameboard.tracks[ city1 ][ city2 ].colorTrack_2;

						nbCardsColor_1 = game.players[0].nbCardsType[ claimedColor_1 ];
						nbCardsColor_2 = game.players[0].nbCardsType[ claimedColor_2 ];

						desiredColor[claimedColor_1]+= nbCardsColor_1;
						desiredColor[claimedColor_2]+= nbCardsColor_2;
						
						i++;
						j++;
					}
				}
				if (nbCardsColor_1 >= lengthTrackClaimed){

					move.type = CLAIM_ROUTE;
					claimRoute(city1, 
							   city2, 
							   game.gameboard.tracks[ city1 ][ city2 ].colorTrack_1,
							   move.claimRoute.nbLocomotives);
					/*
					printf("doing objective :");
					printf(" %d (", city1);
					printCity(city1);
					printf(") -> (");
					printCity(city2);
					printf(") %d \n", city2);
					*/
				}
				else if (nbCardsColor_2 >= lengthTrackClaimed){

					move.type = CLAIM_ROUTE;
					claimRoute(city1, 
							   city2, 
							   game.gameboard.tracks[ city1 ][ city2 ].colorTrack_2,
							   move.claimRoute.nbLocomotives);
					/*
					printf("doing objective :");
					printf(" %d (", city1);
					printCity(city1);
					printf(") -> (");
					printCity(city2);
					printf(") %d \n", city2);
					*/
				}
				else{
//printf("Point 4:\n");	
					maxToMin(desiredColor, priorityOrder,10);
					for(int i=0; i < 5; i++){	
						all = 0;
						while ((game.faceUp[i] != priorityOrder[all]) && (all != 10)){
							all++;
						}
						if (all != 10){
	//printf("Point 4.a:\n");
							move.type = DRAW_CARD;
							move.drawCard.card = priorityOrder[all];
							desiredColor[ priorityOrder[all] ]--;
						}
						else{
	//printf("Point 4.b:\n");
							move.type = DRAW_BLIND_CARD;
						}
					}
				}
			}
			
//printf("Point 5:\n");
			replay = needReplay(&move, lastMove);
			movePlayed = playOurMove(&move, &lastMove, obj);
			updateGame(&game, &move, obj);
//printf("Point 6:\n");


			
				/* finding the shortest track for each new objectives, so when (nbObjectivesListed - game.players[0].nbObjectives) > 0 */ 
				for (int i = nbObjectivesListed; i < game.players[0].nbObjectives; i++){
//printf("Point 7:\n");
			
					findShortestTrack(&game, game.players[0].objectives[i].city1, game.players[0].objectives[i].city2, G, D, Prev);

			
					/* we save array "Prev" generated in "ShortestTrack" and the number of elements contained in Prev */
					printf("Shortest Track for objective number %d:",i+1);
					nbCitiesShortestTrackObjectives = printTrack(Prev, &ShortestTrack[nbObjectivesListed*N], game.players[0].objectives[i].city1, game.players[0].objectives[i].city2);
					
					lengthShortestTrackObjectives[i] = nbCitiesShortestTrackObjectives;

					nbElemShortestTrack+= nbCitiesShortestTrackObjectives;
					nbObjectivesListed++;
				}
//printf("Point 8:\n");
			
			if((replay) && (movePlayed == NORMAL_MOVE)){
//printf("Point 9:\n");				
				//printMap();
				printf("...I play again...\n");
				currentGame(&game);


				if( move.type == DRAW_OBJECTIVES){
//printf("Point 10:\n");					
					for(int i=0; i < 3; i++){
						move.chooseObjectives.chosen[i] = 1;
					}
					move.type = CHOOSE_OBJECTIVES;
				}
				else{
//printf("Point 11:\n");
				maxToMin(desiredColor, priorityOrder,10);
				for(int i=0; i < 5; i++){	
					all = 0;
					while ((game.faceUp[i] != priorityOrder[all]) && (all != 10)){
						all++;
					}
					if (all != 10){
//printf("Point 11.a:\n");
						move.type = DRAW_CARD;
						move.drawCard.card = priorityOrder[all];
						desiredColor[ priorityOrder[all] ]--;
					}
					else{
//printf("Point 11.b:\n");
						move.type = DRAW_BLIND_CARD;
					}
				}
			}
//printf("Point 12:\n");
				replay = needReplay(&move, lastMove);
				movePlayed = playOurMove(&move, &lastMove, obj);
				updateGame(&game, &move, obj);
//printf("Point 13:\n");

			}
			printf("...I am done\n");
			
		}
		else{printf("11\n");
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