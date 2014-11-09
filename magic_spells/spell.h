#ifndef SPELL_H
#define SPELL_H

#define NUM_OF_SPELLS		4	// If someone adds new spell config file in magic_spells folder
					// then set this constant to total number of config files in that folder.
#define CAPACITY_PER_SPELL	5

#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "../maps/map.h"

using namespace std;

typedef struct spell{
	string name;
	int magic;
	int attack_speed;
	int movement_speed; 
	int damage;
	int time;
	int range;
	bool is_Disabled;
	time_t magic_lock;
	path_t path[CAPACITY_PER_SPELL];
	int nitro;
	int index[CAPACITY_PER_SPELL];
}spell;

/**
  * Creates spell struct by reading filename from magic_spells folder. If filename does not exist, game exits.
**/  
spell init_spell(const char *filename);
#endif
