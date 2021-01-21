#include "TicketToRideAPI.h"

/* ask for a move */
void moveChoice(t_move* move);

/* plays the move given as a parameter (send to the server) */
/* returns the return code */
t_return_code playOurMove(t_move* move, t_color* lastCard, t_objective obj[3]);

/* tell if we need to replay */
int needReplay(t_move* move, t_color lastCard);