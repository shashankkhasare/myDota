all : client.o
	g++ -w -g client.o -o ../client -lSDL -lpthread
   
client.o : client.cpp   client.h ../Widget.h ../Entity.h
	g++ -c -w -g client.cpp -lSDL -lpthread

clean : 
	rm client.o 

