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

void read_line_into_points_list(points_list *l, line_t *line){
	int i;
	l->num_of_points = line->capacity/2;
	l->points = (point *) malloc(sizeof(point) * l->num_of_points);

	for(i = 0; i < line->capacity; i+= 2){
		l->points[i/2].x = atoi(line->line[i]);
		l->points[i/2].y = atoi(line->line[i+1]);
	}
}

class Map{
	int rows;
	int cols;
	int war_border_offset; 
	points_list team_a_spawn_border, team_b_spawn_border; 
	char **terrain;
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



/**
  * Creates map by reading filename from map folder. If filename does not exist, game exits.
**/  
Map::Map(char *map_file){
	FILE *fp;

	if((fp = fopen(map_file, "r"))){
		char line[200], a = ' ', *delim;
		int lineno = 1, tmp, tmp2, i, j = 0;
		delim = &a;
		char **lines;
		para_t para;

		while(fgets(line, 200, fp)){
			switch(lineno){
				case 1:
					//ROWS & COLS:
					para = read_paragraph(2, fp, SINGLE_PT, 1, delim, line);
					rows = atoi(para.lines[0].line[0]);
					cols = atoi(para.lines[1].line[0]);
					terrain = (char **) malloc(sizeof(char *)*rows);
					lineno += 2;
					break;

				case 3:
					//WAR_BORDER_OFFSET:
					para = read_paragraph(1, fp, SINGLE_PT, 1, delim, line);
					war_border_offset = atoi(para.lines[0].line[0]);
					lineno += 1;
					break;	
				case 4:
					//TEAM_A_SPAWN_BORDER & TEAM_B_SPAWN_BORDER:
					para = read_paragraph(2, fp, MANY_PTS, 2, delim, line);
					team_a_spawn_border.num_of_points = para.lines[0].capacity/2;
					team_a_spawn_border.points = (point *) malloc(sizeof(point)*team_a_spawn_border.num_of_points);
					team_b_spawn_border.num_of_points = para.lines[1].capacity/2;
					team_b_spawn_border.points = (point *) malloc(sizeof(point)*team_b_spawn_border.num_of_points);

					read_line_into_points_list(&team_a_spawn_border, &para.lines[0]);	
					read_line_into_points_list(&team_b_spawn_border, &para.lines[1]);	
					lineno += 2;
					break;	
				case 6:
					//TEAM_A_TEMPLE_BORDER & TEAM_B_TEMPLE_BORDER:
					para = read_paragraph(2, fp, MANY_PTS, 2, delim, line);
					temple_a.boundary.num_of_points = para.lines[0].capacity/2;
					temple_a.boundary.points = (point *) malloc(sizeof(point)*temple_a.boundary.num_of_points);
					temple_b.boundary.num_of_points = para.lines[1].capacity/2;
					temple_b.boundary.points = (point *) malloc(sizeof(point)*temple_b.boundary.num_of_points);

					read_line_into_points_list(&temple_a.boundary, &para.lines[0]);	
					read_line_into_points_list(&temple_b.boundary, &para.lines[1]);
					lineno += 2;
					break;
				case 8:
					//A_TEAM_TEMPLE_CENTER & B_TEAM_TEMPLE_CENTER:
					para = read_paragraph(2, fp, SINGLE_PT, 2, delim, line);
					temple_a.pos_x = atoi(para.lines[0].line[0]);
					temple_a.pos_y = atoi(para.lines[0].line[1]);;
					temple_b.pos_x = atoi(para.lines[1].line[0]);
					temple_b.pos_y = atoi(para.lines[1].line[1]);;
					lineno += 2;
					break;	
				case 10:
					//MAP:
					lineno += 1;
					break;
				default:
					//Parse Map;
					terrain[j] = (char *) malloc(sizeof(char)*cols);
					for(i=0; i<cols*2; i+=2)
						terrain[j][i/2] = line[i];
					j++;
					break;	
			};
			free_para(&para);	
		}	
	}
	else{
		printf("MAP: Enable to open %s. File does not exist.\n", map_file);
		exit(0);
	}

	fclose(fp);

}

Map::~Map(){
	int i;
	free(team_a_spawn_border.points);
	free(team_b_spawn_border.points);
	for(i=0; i<rows; i++)
		free(terrain[i]);
	free(terrain);		
	
	free(temple_a.boundary.points);
	free(temple_b.boundary.points);
}

void Map::PrintMap(){
	int i;
	printf("ROWS: %d\n", rows);
	printf("COLS: %d\n", cols);
	printf("WAR_BORBER_OFFSET: %d\n", war_border_offset);
	
	printf("TEAM_A_SPAWN_BORDER: %d ", team_a_spawn_border.num_of_points);
	for(i = 0; i<team_a_spawn_border.num_of_points; i++)
		printf("%d %d ", team_a_spawn_border.points[i].x, team_a_spawn_border.points[i].y);
	printf("\n");
	
	printf("TEAM_B_SPAWN_BORDER: %d ", team_b_spawn_border.num_of_points);
	for(i = 0; i<team_b_spawn_border.num_of_points; i++)
		printf("%d %d ", team_b_spawn_border.points[i].x, team_b_spawn_border.points[i].y);
	printf("\n");	
	
	printf("TEAM_A_TEMPLE_BORDER: %d ", temple_a.boundary.num_of_points);
	for(i = 0; i<temple_a.boundary.num_of_points; i++)
		printf("%d %d ", temple_a.boundary.points[i].x, temple_a.boundary.points[i].y);
	printf("\n");
	
	printf("TEAM_B_TEMPLE_BORDER: %d ", temple_b.boundary.num_of_points);
	for(i = 0; i<temple_b.boundary.num_of_points; i++)
		printf("%d %d ", temple_b.boundary.points[i].x, temple_b.boundary.points[i].y);
	printf("\n");
		
	printf("A_TEAM_TEMPLE_CENTER: %d %d\n", temple_a.pos_x, temple_a.pos_y);
	printf("B_TEAM_TEMPLE_CENTER: %d %d\n", temple_b.pos_x, temple_b.pos_y);
	
	printf("MAP:\n");
	for(i=0; i<rows; i++){
		printf("%s\n", terrain[i]);
	}	
}

/**
 * [dest_x, dest_y] is defined by input device i.e. mouse or keyboard.
 * If [dest_x, dest_y] is 'J', 'A', 'B' then simply sets last_path_index to -1 and return.
  * After executing SSSP, if [dest_x, dest_y] turns out to be unreachable point from [curr_x, curr_y],
  * then simply sets last_path_index to -1 and return.
  * If [dest_x, dest_y] is 'a' or 'b', then 1st feasible point, of all points sorted by euclidean distance from (curr_x, curr_y),
  * defining boundary of corresponding temple, is considered to find path.
  * Writes the path(i.e sequence of [x,y] points) between [curr_x, curr_y] &
  * [dest_x, dest_y] into path_t.path and updates path_t.last_path_index. 
**/  
path_t Map::get_shortest_path(int src_x, int src_y, int dest_x, int dest_y){

}

path_t Map::get_shortest_path(point_t src, point_t dest){
	return get_shortest_path(src.x, src.y , dest.x, dest.y);
}

/**
  * Returns euclidean distance between (x1, y1) and (x2, y2).
**/  
float Map::euclidean_dist(int x1, int y1, int x2, int y2){


}

float Map::euclidean_dist(point_t src, point_t dest){
	return euclidean_dist(src.x, src.y , dest.x, dest.y);
}

/**
  * Returns 1 if straight line (constucted using naive_line_drawaing_algo() Source: Wikipedia) 
  * between (x1, y1) and (x2, y2) has only '.'s. Else return 0;
**/  
bool Map::is_line_of_sight_clear(int x1, int y1, int x2, int y2){

}

bool Map::is_line_of_sight_clear(point_t src, point_t dest){
	return is_line_of_sight_clear(src.x, src.y , dest.x, dest.y);
}
#endif
