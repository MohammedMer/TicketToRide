#include "TicketToRideAPI.h"
#define N  NB_MAX_CITIES

int minLenght(int* D, int* Traveled);
void findShortestTrack(t_game* game, int src, int dest, int G[N][N], int* D, int* Prec);
int printTrack(int* Prec, int* ShorterTrack, int src, int dest);
int maxToMin(int* desiredColor, int* priorityOrder, int stop);