#ifdef MAP_H

#define TEMPLE_BORDER		'T'
#define JUNGLE_TREE		'J'
#define TEAM_A_SPAWN_SYM	'A'
#define TEAM_B_SPAWN_SYM	'B'
#define FREE_SPACE		'.'

#define TEAM_A			0
#define TEAM_B			1

typedef struct map_t{
	int rows;
	int cols;
	char **terrain;
	char **adj_mat;
	temple temple_a, temple_b;
	char spawn_locations[2][2];
}map;

typedef struct path_t{
	char path[2500][2];
	int last_path_index;
}path_t;

typedef struct point_t{
	int x, y;
}point;

typedef struct points_list{
	point * points;
	int num_of_points;
}points_list;


/**
  * Creates map by reading filename from map folder. If filename does not exist, game exits.
**/  
map * init_map(char *filename);

/**
  * Returns m->temple_location[index].
**/  
char * map_temple_location(int index, map *m);

/**
  * [dest_x, dest_y] is defined by input device i.e. mouse or keyboard.
  * If [dest_x, dest_y] is 'J', 'A', 'B' then simply sets last_path_index to -1 and return.
  * If [dest_x, dest_y] is unreachable point from [curr_x, curr_y], then simply sets last_path_index to -1 and return.
  * If [dest_x, dest_y] is 'a' or 'b', then 1st feasible point, of all points sorted by euclidean distance from (curr_x, curr_y),
  * defining boundary of corresponding temple, is considered to find path.
  * Writes the path(i.e sequence of [x,y] points) between [curr_x, curr_y] &
  * [dest_x, dest_y] into path_t.path and updates path_t.last_path_index. 
**/  
path_t construct_path(map *m, int curr_x, int curr_y, int dest_x, int dest_y);

/**
  * Returns euclidean distance between (x1, y1) and (x2, y2).
**/  
float euclidean_dist(int x1, int y1, int x2, int y2);

/**
  * Returns 1 if straight line (constucted using naive_line_drawaing_algo() Source: Wikipedia) 
  * between (x1, y1) and (x2, y2) has only '.'s. Else return 0;
**/  
int is_line_of_sight_clear(int x1, int y1, int x2, int y2);
#endif
