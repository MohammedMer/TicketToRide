game: game.o clientAPI.o TicketToRideAPI.o
	gcc -Wall -o game game.o clientAPI.o TicketToRideAPI.o

game.o:
	gcc -Wall -c game.c

clientAPI.o:
	gcc -Wall -c clientAPI.c

TicketToRideAPI.o:
	gcc -Wall -c TicketToRideAPI.c

clean:
	rm -f game.o game clientAPI.o TicketToRideAPI.o