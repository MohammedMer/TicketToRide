#include "TicketToRideAPI.h"

void initGame(t_game* game, char* serverName, unsigned int port, char* gameType);
void initMap(t_game* game, t_color* ourCards);
void players(t_game* game, t_color ourCards[4]);
//t_return_code info(t_game* game);
void currentGame(t_game* game);
void updateGame(t_game* game, t_move* move, t_objective obj[3]);