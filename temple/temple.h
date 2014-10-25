#ifdef TEMPLE_H
#include "../maps/map.h" 

#define DEMOLISHED	0
#define PRESERVED	1

typedef struct temple_t{
	int pos_x, pos_y;
	points_list boundary;
	int health;
	int max_health;
	int state;
}temple;

/**
  * Returns pointer to malloced temple. init_temple() will be called by init_map() directly or indirectly.
  * All neccessary information is in map header of map file.
**/  
temple * init_temple();

/**
  * Updates the t->health by update. t->health is limited between [0, max_health].
**/  
void update_health(temple *t, int update);

/**
  * Returns points_list of temple.boundary sorted by euclidean distance between [x,y] and points in temple.boundary .
**/  
points_list * get_sorted_temple_boundary_points(temple *p, int x, int y);
#endif
