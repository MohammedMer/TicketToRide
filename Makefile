game: game.o  clientAPI.o TicketToRideAPI.o data_game.o algo.o move.o
	gcc -Wall -o game game.o data_game.o move.o algo.o clientAPI.o TicketToRideAPI.o

game.o: game.c game.h clientAPI.h TicketToRideAPI.h data_game.h algo.h move.h
	gcc -Wall -c game.c


game_solo: game_solo.o clientAPI.o TicketToRideAPI.o data_game.o move.o
	gcc -Wall -o game_solo game_solo.o data_game.o move.o clientAPI.o TicketToRideAPI.o

game_solo.o: game_solo.c game_solo.h clientAPI.h TicketToRideAPI.h data_game.h algo.h move.h
	gcc -Wall -c game_solo.c


clientAPI.o: clientAPI.c clientAPI.h
	gcc -Wall -c clientAPI.c

data_game.o: data_game.c data_game.h
	gcc -Wall -c data_game.c

algo.o: algo.c algo.h
	gcc -Wall -c algo.c	

move.o: move.c move.h
	gcc -Wall -c move.c

TicketToRideAPI.o: TicketToRideAPI.c TicketToRideAPI.h clientAPI.h
	gcc -Wall -c TicketToRideAPI.c

clean:
	rm -f game.o game game_solo.o game_solo algo.o data_game.o move.o clientAPI.o TicketToRideAPI.o