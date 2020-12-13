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

	t_gameboard gameboard;
	t_game game;


	char gameType[100] = "TRAINING DO_NOTHING timeout=10 start=0 map=USA";
    char gameName[20] = "Mohammeds_bot";

	waitForT2RGame(gameType,gameName,&gameboard.nbCities,&gameboard.nbTracks);
	printf("Données récupérées !\n");

	gameboard.tracks = (int*) malloc(5*gameboard.nbTracks*sizeof(int));
	t_color cards[4];

	getMap(gameboard.tracks,game.faceUp,cards);
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