#ifdef ITEM_H
typedef struct item{
	int item_id;
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
item * init_item(char *filename);
#endif
