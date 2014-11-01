#ifndef MAP_H
#define MAP_H

#include "../parser/parser.h"


#define TEMPLE_BORDER		'T'
#define JUNGLE_TREE		'J'
#define TEAM_A_SPAWN_SYM	'A'
#define TEAM_B_SPAWN_SYM	'B'
#define FREE_SPACE		'.'

#define TEAM_A			0
#define TEAM_B			1

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


typedef struct temple_t{
	int pos_x, pos_y;
	points_list boundary;
	int health;
	int max_health;
	int state;
}temple;


class Map{
	int rows;
	int cols;
	int war_border_offset; 
	points_list team_a_spawn_border, team_b_spawn_border; 
	char terrain[75][75];
	//char **adj_mat;
	temple temple_a, temple_b;
	char spawn_locations[2][2];

	public:

	path_t get_shortest_path(int src_x, int src_y, int dest_x, int dest_y);
	path_t get_shortest_path(point_t src, point_t dest);
	float euclidean_dist(int x1, int y1, int x2, int y2);
	float euclidean_dist(point_t src, point_t dest);
	bool is_line_of_sight_clear(int x1, int y1, int x2, int y2);
	bool is_line_of_sight_clear(point_t src, point_t dest);
	void PrintMap();
	
	Map(char * filename );
	~Map();
};



#endif
