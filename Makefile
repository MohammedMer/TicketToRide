game: game.o clientAPI.o TicketToRideAPI.o
	gcc -Wall -o game game.o clientAPI.o TicketToRideAPI.o

game.o: game.c game.h clientAPI.h TicketToRideAPI.h
	gcc -Wall -c game.c

clientAPI.o: clientAPI.c clientAPI.h
	gcc -Wall -c clientAPI.c

TicketToRideAPI.o: TicketToRideAPI.c TicketToRideAPI.h clientAPI.h
	gcc -Wall -c TicketToRideAPI.c

clean:
	rm -f game.o game clientAPI.o TicketToRideAPI.o