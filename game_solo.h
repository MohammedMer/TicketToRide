#include "TicketToRideAPI.h"

#define NB_MAX_CITIES 40

/* Question 3 */

/* gameboard's definition */
typedef struct{
	int city1, city2;
	int lengthTrack;
	t_color colorTrack_1, colorTrack_2;
	int taken;
} t_tracks;

typedef struct{
	int nbCities; /* number of cities */
	int nbTracks; /* number of tracks */
	t_tracks tracks[NB_MAX_CITIES][NB_MAX_CITIES]; /* array of int for existing tracks */
} t_gameboard;

/* player's definition */
typedef struct{
	char name[20];
	int nbLocomotives;			/* number of Locomotives owned*/
	int nbCards;				/* number of cards owned */		
	int nbCardsType[10];		/* number of each card type (color) owned */
	int nbObjectives;			/* number of objectives owned */
	t_objective objectives[20]; /* differents types of objectives owned */
} t_player;

/* game's description */
typedef struct{
	char gameName[20];
	int curentPlayer;		/* number of the player who is playing*/
	int playerId;			/* id of the player, playerId == 0 -> we play, playerId == 1 -> the opponent plays */
	t_color faceUp[5];		/* color of face_up cards */
	t_gameboard gameboard;	/* the gameboard */
	t_player players[2];	/* array of the 2 players */
} t_game;