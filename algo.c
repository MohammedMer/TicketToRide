#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "game.h"

#define N  NB_MAX_CITIES
	
int minLenght(int* D, int* Traveled){
	/* initialization */
	int min = INT_MAX;
	int ind_min;

	/* search for the nearest unvisited city */
	for (int i=0; i < N; i++){
		if ((Traveled[i] == 0) && (D[i] < min)){
			min = D[i];
			ind_min = i;
		}
	}
	return ind_min;
}

void findShortestTrack(t_game* game, int src, int dest, int G[N][N], int* D, int* Prev, int* nbCitiesPrec){
		/* Dijkstra's algorithm */

	int* Traveled = (int*) malloc(N*sizeof(int));
	int u;
	/* initialization */
	for (int i=0; i < N; i++){
		D[i] = INT_MAX;
		Traveled[i] = 0;
	}
	/* we start from the source */
	D[src] = 0;

	/* We are looking for the shortest way to go from src to each city */
	while (u != dest){
		/* the nearest uninvited city is found */
		u = minLenght(D, Traveled);
		/* we mark this city as visited */
		Traveled[u] = 1; /* 1 <=> True */
		/* Each city linked to this city is updated */
		for (int v=0; v < N; v++){
			/* we update only unvisited cities, where there is a path between u and v and where it is shorter to reach v trought u */
			if ((Traveled[v] == 0) && (G[u][v] != INT_MAX) /*&& (game->gameboard.tracks[u][v].taken != 1)*/){
				D[v] = D[u] + G[u][v];
				Prev[v] = u;
			}
		}
	}
}

/* return number of elements in array "Prev" and create a copy of "Prev" in "ShortestTrack"*/
int printTrack(int* Prev, int* ShortestTrack, int src, int dest){
	/* we start from the end */
	int v = dest;
	ShortestTrack[0] = v;

	printf("\t");
	printCity(v);

	int i = 1;
	/* we go back up the track until we reach the starting point */
	while (v != src){
		/* print the track from Prev[v] to v*/
		v = Prev[v];
		ShortestTrack[i] = v;

		printf("->");
		printCity(v);

		i++;
	}
	printf("\n");

	return i;
}

int maxToMin(int* desiredColor, int* priorityOrder, int arrayLenght){
	int max = desiredColor[0];
	for (int i=0; i< arrayLenght; i++){
		if (desiredColor[i] > max)
			max = desiredColor[i];
	}

	priorityOrder[0] = max;
	arrayLenght--;

	if(arrayLenght!=0)
		return maxToMin(&desiredColor[1], &priorityOrder[1],arrayLenght);
	else
		return 0;
}