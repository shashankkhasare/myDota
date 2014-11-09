#ifndef ITEM_H
#define ITEM_H

#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

typedef struct item{
	string image_file;
	int movement_speed;
	int attack_speed;
	int health;
	int damage_melee;
	int damage_magic;
	int temp_heal;
	int required_space;
	int quantity;
}item;

/**
  * Creates item by reading filename from items folder. If filename does not exist, game exits.
**/  
item init_item(const char *filename);
#endif
