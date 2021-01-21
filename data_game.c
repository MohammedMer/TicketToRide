#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "TicketToRideAPI.h"
#include "game.h"

#define N  NB_MAX_CITIES
/* initialization of game parameters */
void initGame(t_game* game, char* serverName, unsigned int port, char* gameType){	
	
	/* connection to server */
	printf("Successful server connection)\n");
	printf("Enter the player's name: \n\t");
	scanf("%s", game->players[0].name);

	char answer[1];
	printf("Do you want to play vs a bot ? : enter 'y' or 'n' \n\t");
	scanf("%s", answer);

	if (*answer == 'y'){
		printf("Enter a game name: \n\t");
		scanf("%s", game->gameName);
	}


	char gameName[20] = "tesT_bot";

	connectToServer(serverName,port, game->players[0].name);
	/* wait for a game and get the map */
	waitForT2RGame(gameType,gameName,&(game->gameboard.nbCities),&(game->gameboard.nbTracks));
	printf("Data collected !\n");
}

/* initialization of game map and gets traks informations */
void initMap(t_game* game, t_color* ourCards){
	
	int* arrayTracks = (int*) malloc(5*game->gameboard.nbTracks*sizeof(int));
	game->curentPlayer = getMap(arrayTracks ,game->faceUp,ourCards);

	t_tracks* track = malloc(sizeof(t_tracks));
	t_tracks* interTrack = malloc(sizeof(t_tracks));


	/*int N = game->gameboard.nbTracks;*/
	for (int i=0; i < N; i++){
		for (int j=0; j < N; j++){
			game->gameboard.tracks[i][j].lengthTrack = 0;
		}
	}
	
	for (int i=0; i < game->gameboard.nbTracks; i++){

		track->city1 = arrayTracks[i*5];
		track->city2 = arrayTracks[i*5+1];
		track->lengthTrack = arrayTracks[i*5+2];
		track->colorTrack_1 = arrayTracks[i*5+3];
		track->colorTrack_2 = arrayTracks[i*5+4];

		interTrack->city1 = track->city1;
		interTrack->city2 = track->city2;
		interTrack->lengthTrack = track->lengthTrack;
		interTrack->colorTrack_1 = track->colorTrack_1;
		interTrack->colorTrack_2 = track->colorTrack_2;
		interTrack->taken = 0;

		game->gameboard.tracks[track->city1][track->city2] = *interTrack;
		game->gameboard.tracks[track->city2][track->city1] = *interTrack;
	}

	printMap();
}

/* initialization of players informations */
void players(t_game* game, t_color ourCards[4]){

	game->players[0].nbLocomotives = 45;
	game->players[1].nbLocomotives = 45;

	game->players[0].nbCards = 4;
	game->players[1].nbCards = 4;

	for(int i=0; i < 10; i++){
		game->players[0].nbCardsType[i] = 0;
		game->players[1].nbCardsType[i] = 0;
	}

	/* we don't know what kind of cards the opponent have */
	for(int i=0; i < 4; i++){
		(game->players[0]).nbCardsType[ (int) ourCards[i] ]++;
	}
		
	game->players[0].nbObjectives = 0;
	game->players[1].nbObjectives = 0;
}

/* gets name of the color from an int */
char* cardColor(int color_int){
	char* color_char = (char*) malloc(10*sizeof(char));

	switch(color_int){
		case(0):
			color_char = "NONE";
			break;
		case(1):
			color_char = "PURPLE";
			break;
		case(2):
			color_char = "WHITE";
			break;
		case(3):
			color_char = "BLUE";
			break;
		case(4):
			color_char = "YELLOW";
			break;
		case(5):
			color_char = "ORANGE";
			break;
		case(6):
			color_char = "BLACK";
			break;
		case(7):
			color_char = "RED";
			break;
		case(8):
			color_char = "GREEN";
			break;
		case(9):
			color_char = "MULTICOLOR";
			break;
	}
	return color_char;
}

/* prints game's informations */
void currentGame(t_game* game){
	/* printing of our informations*/
	if (game->curentPlayer == 0){
		char* color = (char*) malloc(10*sizeof(char));

		/* printing of face Up cards*/
		printf("Face Up cards: \n");
		for(int i=0; i < 5; i++){

			color = cardColor(game->faceUp[i]);
			printf("\t color: (%d) %s\n", (int) game->faceUp[i], color);
		}
		/* printing of cards owned*/
		printf("Cards owned (%d): \n",game->players[0].nbCards);
		for(int i=0; i < 10; i++){

			color = cardColor(i);
			/* when we have a card of the current color, we print how many card of this color we have */
			if(game->players[0].nbCardsType[i])
				printf("\t color: (%d) %d * %s\n", i, game->players[0].nbCardsType[i], color);
		}
		/* printing of objectives owned*/
		printf("Objectives owned : \n");
		for(int i=0; i < game->players[0].nbObjectives; i++){
			printf("%d. %d (", i+1, game->players[0].objectives[i].city1);
			printCity(game->players[0].objectives[i].city1);
			printf(") -> (");
			printCity(game->players[0].objectives[i].city2);
			printf(") %d (%d pts)\n", game->players[0].objectives[i].city2, game->players[0].objectives[i].score);
		}
	}
	/* printing of opponent's cards' number*/
	else{
			printf("The opponent has %d cards. \n",game->players[1].nbCards);
		}
	printf("\n");
}

/* */
void updateGame(t_game* game, t_move* move, t_objective obj[3]){
	/* Updating objectives owned */
	int j = 0;
	if (move->type == CHOOSE_OBJECTIVES){
		for(int i=0; i < 3; i++){
			if(move->chooseObjectives.chosen[i] == 1){
				game->players[ game->curentPlayer ].objectives[ game->players[0].nbObjectives + j ].city1 = obj[i].city1;
				game->players[ game->curentPlayer ].objectives[ game->players[0].nbObjectives + j ].city2 = obj[i].city2;
				game->players[ game->curentPlayer ].objectives[ game->players[0].nbObjectives + j ].score = obj[i].score;
				j++;
			}
		}
		(game->players[0]).nbObjectives += move->chooseObjectives.nbObjectives;
	}

	/* Updating faceUp cards */
	for(int i=0; i < 5; i++){
		game->faceUp[i] = move->drawCard.faceUp[i];
	}

	/* Updating players' cards owned when we draw a card */
	if (move->type == DRAW_CARD){
		(game->players[ game->curentPlayer ]).nbCardsType[ (int) move->drawCard.card ]++;
		(game->players[ game->curentPlayer ]).nbCards++;

	}
	/* Updating players' cards owned when we draw a blind card */
	if (move->type == DRAW_BLIND_CARD){
		(game->players[ game->curentPlayer ]).nbCardsType[ (int) move->drawBlindCard.card ]++;
		(game->players[ game->curentPlayer ]).nbCards++;
	}
	/* Updating players' cards owned, number of locomotive and state of route (taken or not) when claim a route */
	if (move->type == CLAIM_ROUTE){
		(game->players[ game->curentPlayer ]).nbCardsType[ (int) move->claimRoute.color ]-= move->claimRoute.nbLocomotives;
		(game->players[ game->curentPlayer ]).nbLocomotives-= move->claimRoute.nbLocomotives;
		game->gameboard.tracks[move->claimRoute.city1][move->claimRoute.city2].taken = 1;
	}
}