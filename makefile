CC = g++
EXEC_NAME = mydota
BASIC_C = main.cpp
OPENGL_FLAGS = -lSDL

all : server menu client

server : subparts
	g++ game_server/game_server.o player/player.o heros/hero.o maps/map.o items/item.o magic_spells/spell.o  -o gameserver -lpthread

menu: menu.o
	g++ -w -g main.o -o mydota -lSDL
menu.o :main.cpp
	g++ -c -w main.cpp -lSDL

subparts:
	cd heros ; make
	cd player; make
	cd maps; make
	cd game_server; make
	cd items; make
	cd magic_spells; make

client: game_client/client.cpp game_client/client.h Entity.h Widget.h 
	cd game_client; make

clean:
	cd heros ; make clean
	cd player; make clean
	cd maps; make clean
	cd items; make clean
	cd magic_spells; make clean
	cd game_server; make clean
	cd game_client; make clean
	rm  mydota gameserver client main.o 

kill:
	kill -9 `pidof gameserver` &
	kill -9 `pidof client` &
	kill -9 `pidof mydota` &
