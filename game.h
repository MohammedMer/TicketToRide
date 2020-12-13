/* Question 3 */

/* gameboard's definition */
typedef struct{
	int nbCities; /* number of cities */
	int nbTracks; /* number of tracks */
	int* tracks;  /* array of existing tracks */
} t_gameboard;

/* player's definition */
typedef struct{
	int nbLocomotives;			/* number of Locomotives owned*/
	int nbCards;				/* number of cards owned */		
	int nbCardsType[9];			/* number of each card type (color) owned */
	int nbObjectives;			/* number of objectives owned */
	t_objective objectives[20]; /* differents types of objectives owned */
} t_player;

/* game's description */
typedef struct{
	int curentPlayer;		/* number of the player who is playing*/
	int PlayerId;			/* id of the player */
	t_color faceUp[5];		/* color of face_up cards */
	t_gameboard gameboard;	/* the gameboard */
	t_player players[2];	/* array of the 2 players */
} t_game;