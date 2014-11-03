#ifndef MAP_H
#define MAP_H

#include <math.h>
#include <stdlib.h>
#include <queue>  
#include <stack>  
#include <fstream>
#include <string>
#include <iostream>

#define TEAM_A			0
#define TEAM_B			1

#define DEMOLISHED	0
#define PRESERVED	1

using namespace std;

typedef struct openlist_item{
	int h, g, x, y;
}op_item;

typedef struct path_t{
	char path[2500][2];
	int last_path_index;
}path_t;

typedef struct point_t{
	int x, y;
}point;

typedef struct points_list{
	point points[1500];
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
	//public:
	int rows;
	int cols;
	int war_border_offset; 
	points_list team_a_spawn_border, team_b_spawn_border; 
	char terrain[75][75];
	//char **adj_mat;
	temple temple_a, temple_b;
	point spawn_locations[2];

	public:

	//path_t get_shortest_path(int src_x, int src_y, int dest_x, int dest_y);
	path_t get_shortest_path(point src, point dest);
	float euclidean_dist(int x1, int y1, int x2, int y2);
	float euclidean_dist(point src, point dest);
	bool is_line_of_sight_clear(int x1, int y1, int x2, int y2);
	bool is_line_of_sight_clear(point src, point dest);
	bool is_empty_location(int x, int y);
	char get_location_content(point t);
	void modify_location_content(point t, char c);
	void PrintMap();
	
	Map(char * filename );
	~Map();
};



#endif
