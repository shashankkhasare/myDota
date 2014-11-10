#ifndef HERO_H
#define HERO_H

#define NUM_OF_ITEMS		7

#define STEADY			1
#define MOVING			2
#define MOVING_TO_COLLECT	3

#define MOVING_TO_ATTACK_TEMPLE 4
#define MOVING_TO_ATTACK_HERO	5

#define MELEE			1
#define MAGIC			2

#define TARGET_TYPE_HERO	1
#define TARGET_TYPE_TEMPLE	2

#include "../items/item.h"
#include "../maps/map.h"
#include "../magic_spells/spell.h"
#include "../common/ipc.h"
#include <queue>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;

typedef struct requests{
	bool valid;
	int int1, int2;
}requests;

typedef struct item_bag{
	int bag_capacity;
	int curr_capacity;
	item items[NUM_OF_ITEMS];
}item_bag;

class  Hero{
	public : 
	int hid;
	char symbol_on_map;
	spell mpower;
	int teamid;
	
	int target_id, target_type;
	
	bool isMovementDisabled;
	time_t movement_lock;
	
	bool isMeleeDisabled;
	time_t melee_lock;
	
	int health;
	int max_health;
	int movement_speed;
	int attack_speed;
	int attack_mode;
	int damage;
	
	item_bag bag;
	
	point curr_pos, dest_pos;
	int state;
	path_t path;
	int curr_path_index;	
	queue<cmd_t> inst;
	
	/**
	 * Creates hero by reading filename from heros folder. If filename does not exist, game exits.
	 **/  
	Hero(int h_id, char symbol, point curr, int team);
	Hero();
	
	void drive();
	void process_dfa();
	void spawn(Map *m);
	void route(Map *m, point dest, int option);
	void go_to_steady_state();
};
#endif
