#ifndef HERO_H
#define HERO_H

#define NUM_OF_ITEMS		9

#define STEADY			1
#define MOVING_TO_COLLECT	2
#define MOVING_TO_ATTACK	3
#define CASTING_SPELL		4
#define DISABLED_MAGIC_5_SEC	5
#define STUNNED_FOR_1_SEC	6
#define MOVING			7
#define DEAD			8

#define MELEE_WEAPON		1
#define MAGIC			2

#define	UP			1
#define	DOWN			2
#define	LEFT			3
#define	RIGHT			4

#include "../items/item.h"
#include "../maps/map.h"

typedef struct item_bag{
	int bag_capacity;
	int curr_capacity;
	item items[NUM_OF_ITEMS];
}item_bag;

class  Hero{
	public : 
	int hid;
	char symbol_on_map;
	int team_tag;
	int mpower_id;	//TODO: This will be replaced by magic_t variable in future.
	int health = 97;
	int max_health;
	int movement_speed;
	int attack_speed;
	int attack_mode;
	int damage;
	item_bag bag;
	point_t curr_pos, dest_pos;
	int state;
	int orientation;
	path_t path;
	int curr_path_index;	
	//TODO: Maybe we can add input_buffer, function pointer drive here only. After this, there won't be any player folder.
	


	/**
	 * Creates hero by reading filename from heros folder. If filename does not exist, game exits.
	 **/  
	Hero(int hid);
	Hero();
	void init(int h); 
	

	/**
	 * Adds new_item to item_bag of h. Does not add item_new if it exceeds item_bag.bag_capacity.
	 **/  
	void add_to_item_bag(item *new_item);

	/**
	 * Updates h->curr_x_pos, h->curr_y_pos to x,y respectively. Reads the global State.MAP (TODO: Define state).
	 * Retains old values if, new (x,y) tuple is occupied by some non-collectible (i.e. other hero or tree) in MAP, or (x,y) lies outside of MAP. 
	 * Return -1 on failure of updation, 1 on successful updation.
	 **/
	int update_current_pos(int x, int y);

	/**
	 * Updates h->dest_x_pos, h->dest_y_pos to x,y respectively. Reads the global State.MAP (TODO: Define state).
	 **/

	void update_dest_pos( int x, int y);

	/**
	 * Updates h->health by integer(+ or -) update. The updated is limited to [0, h->max_health]. 
	 * Updates h->state to DEAD if new h->health reaches 0.
	 **/  
	int update_health(int update);

	/**
	 * Updates h->movement_speed by integer(+ or -) update. The updated is limited to [1, 2^(sizeof_int_in_bits()-1)].
	 **/  
	int update_movement_speed( int update);


	/**
	 * Updates h->attack_speed by integer(+ or -) update. The updated is limited to [1, 2^(sizeof_int_in_bits()-1)].
	 **/  
	int update_attack_speed( int update);

	/**
	 * Returns the current position of h.
	 **/  
	point_t get_curr_pos();

	/**
	 * Renders the hero on screen.
	void render_hero(hero *h);
	 **/  
};
#endif
