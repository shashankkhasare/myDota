all : client.o
	g++ -w client.o -o ../client -lSDL
   
client.o : client.cpp   client.h
	g++ -c -w client.cpp -lSDL

