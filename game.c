#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"
#include "game.h"

int main(){

	char* serverName = "li1417-56.members.linode.com";
	unsigned int port = 1234;
	char* name = (char*) malloc(50*sizeof(char));
	name = "Mohammeds_bot";

	connectToServer(serverName,port,name);
	printf("Connexion au serveur réussie x)\n");


	/* Création de la partie, de la carte, des joueurs */

	t_game game;
	/* Je commence la partie */


	void initGame(t_game game,nbLocomotives,nbCards){
		game.curentPlayer = 1;
		game.PlayerId = 1;

		game.players[1].nbLocomotives = 45;
		game.players[2].nbLocomotives = 45;

		game.players[1].nbCards = 4;
		game.players[2].nbCards = 4;
	/*
		game.players[1].nbCardsType[0->8];
		game.players[2].nbCardsType[0->8&];
	*/
		game.players[1].nbObjectives = 0;
		game.players[2].nbObjectives = 0;

		ChooseObjectives(int objectiveCards[3]);

		drawCard(t_color card, t_color deck[5]);
	}

	initGame(game,45,);


	t_return_code moveChoice(t_game game){

		printf("Que veux tu faire ?\n");
		printf("   Prendre:\n");
		printf("\t 1: possession d'une route\n");
		printf("\t 2: une carte de la pioche\n");
		printf("\t 3: une carte face visible\n");
		printf("\t 4: une des cartes objectifs\n");

		int choice;
		scanf("%d",&choice);

		switch(choice){
			case 1:
				return claimRoute(int city1, int city2, int color, int nbLocomotives);
			case 2:
				return drawBlindCard(t_color* card);
			case 3:
				return drawCard(t_color card, t_color deck[5]);
			case 4:
				return drawObjectives(&(game.playeur.objectives[game.playeur.nbObjectives]));
		}

	}

	moveChoice(game);


	char gameType[100] = "TRAINING DO_NOTHING timeout=10 start=0 map=USA";
    char gameName[20] = "Mohammeds_bot";

    /* initialisation des données de la partie */


	waitForT2RGame(gameType,gameName,&game.gameboard.nbCities,&game.gameboard.nbTracks);
	printf("Données récupérées !\n");

	game.gameboard.tracks = (int*) malloc(5*game.gameboard.nbTracks*sizeof(int));
	t_color cards[4];


	getMap(game.gameboard.tracks,game.faceUp,cards);
	printMap();

	t_move move;
	t_return_code play = NORMAL_MOVE;
	int replay = 0;
	t_color card;

	int turn = 1;
	while(play == NORMAL_MOVE){
		printf("Tour n° %d:\n", turn);

		printf("Je joue... \n");
		play = drawBlindCard(&card);
		if(play == NORMAL_MOVE){
			printf("...je rejoue...\n");
			play = drawBlindCard(&card);
		}
		printf("...j'ai terminé\n");
		/*On affiche n'affiche pas la map si la partie est fini pour éviter le message d'erreur suivant: "Error: The server does not acknowledge, but answered: Bad protocol, should send 'WAIT_GAME %s' command" */
		if(play == NORMAL_MOVE){
			printMap();				
		}
			if(play == NORMAL_MOVE){
				printf("L'ordi joue: loading...\n");
				play = getMove(&move,&replay);
			}
			/*L'ordinateur rejoue selon la valeur du booléen replay et si le mouvement precedent n'engendre pas la fin de la partie*/
			while((replay) && (play == NORMAL_MOVE) ){
				printf("...l'ordi rejoue...\n");
				play = getMove(&move,&replay);
			}
			printf("...l'ordi a terminé.\n");

			/*On affiche n'affiche pas la map si la partie est fini pour éviter le message d'erreur suivant: "Error: The server does not acknowledge, but answered: Bad protocol, should send 'WAIT_GAME %s' command" */
			if(play == NORMAL_MOVE){
				printMap();				
			}

		turn++;
	}
	printf("Partie terminé.\n");

	printf("Déconnexion du serveur...\n");
	closeConnection();

	return 0;
}