#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"


int main(){

	char* serverName = "li1417-56.members.linode.com";
	unsigned int port = 1234;
	char* name = (char*) malloc(50*sizeof(char));
	name = "Mohammeds_bot";

	connectToServer(serverName,port,name);
	printf("Connexion au serveur réussie x)\n");

	char gameType[100] = "TRAINING DO_NOTHING timeout=10 start=0 map=USA";
    char gameName[20] = "Mohammeds_bot";
	int nbCities;
	int nbTracks;

	waitForT2RGame(gameType,gameName,&nbCities,&nbTracks);
	printf("Données récupérées !\n");

	int* tracks = (int*) malloc(5*nbTracks*sizeof(int));
	t_color faceUp[5];
	t_color cards[4];

	getMap(tracks,faceUp,cards);

	printf("Déconnexion du serveur...\n");
	closeConnection();

	return 0;
}