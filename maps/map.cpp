#include "map.h"

bool operator<(const op_item& l, const op_item& r)  
{  
	if(l.h > r.h)
		return true;
	else if(l.h == r.h && l.g > r.g)
		return true;
	else if(l.h == r.h && l.g == r.g && l.x > r.x)
		return true;
	else if(l.h == r.h && l.g == r.g && l.x == r.x && l.y > r.y)
		return true;
	else if(l.h == r.h && l.g == r.g && l.x == r.x && l.y == r.y)
		return true;	
	else			
		return false;  
}  

/**
  * Creates map by reading filename from map folder. If filename does not exist, game exits.
**/  
Map::Map(char *map_file){
	ifstream infile(map_file);
	
	if(!infile.good()){
		cout << "Map file " << map_file << " doesn't exist. Exiting.\n";
		exit(0);
	}
	
	string token;
	int i, j;
	while(infile >> token){
		if(token == "ROWS:"){
			infile >> rows;
		}
		else if(token == "COLS:"){
			infile >> cols;
		}
		else if(token == "WAR_BORDER_OFFSET:"){
			infile >> war_border_offset;
		}
		else if(token == "TEAM_A_SPAWN_BORDER:"){
			infile >> team_a_spawn_border.num_of_points;
			for(i = 0; i<team_a_spawn_border.num_of_points; i++)
				infile >> team_a_spawn_border.points[i].x >> team_a_spawn_border.points[i].y;
			
		}
		else if(token == "TEAM_B_SPAWN_BORDER:"){
			infile >> team_b_spawn_border.num_of_points;
			for(i = 0; i<team_b_spawn_border.num_of_points; i++)
				infile >> team_b_spawn_border.points[i].x >> team_b_spawn_border.points[i].y;
			
		}
		else if(token == "TEAM_A_TEMPLE_BORDER:"){
			infile >> temple_a.boundary.num_of_points;
			for(i = 0; i<temple_a.boundary.num_of_points; i++)
				infile >> temple_a.boundary.points[i].x >> temple_a.boundary.points[i].y;
		}
		else if(token == "TEAM_B_TEMPLE_BORDER:"){
			infile >> temple_b.boundary.num_of_points;
			for(i = 0; i<temple_b.boundary.num_of_points; i++)
				infile >> temple_b.boundary.points[i].x >> temple_b.boundary.points[i].y;
		}
		else if(token == "A_TEAM_TEMPLE_CENTER:"){
			infile >> temple_a.pos_x >> temple_a.pos_y;
		}
		else if(token == "B_TEAM_TEMPLE_CENTER:"){
			infile >> temple_b.pos_x >> temple_b.pos_y;
		}
		else if(token == "MAP:"){
			for(i=0; i<rows; i++)
				for(j=0; j<cols; j++)
					infile >> terrain[i][j];
		}
	}
	spawn_locations[0] = {0, rows-1};
	spawn_locations[1] = {cols-1, 0};
	temple_a.health = temple_b.health = 2000;
	temple_a.max_health = temple_b.max_health = 2000;
	temple_a.state = temple_b.state = PRESERVED;
	
	infile.close();	
}

Map::~Map(){
	
}

void Map::PrintMap(){
	int i, j;
	
	printf("ROWS:  %d \n", rows);
	printf("COLS:  %d \n", cols);
	printf("WAR_BORDER_OFFSET:  %d \n", war_border_offset);
	
	printf("TEAM_A_SPAWN_BORDER:  %d ", team_a_spawn_border.num_of_points);
	for(i = 0; i<team_a_spawn_border.num_of_points; i++)
		printf("%d %d ", team_a_spawn_border.points[i].x, team_a_spawn_border.points[i].y);
	printf("\n");
	
	printf("TEAM_B_SPAWN_BORDER:  %d ", team_b_spawn_border.num_of_points);
	for(i = 0; i<team_b_spawn_border.num_of_points; i++)
		printf("%d %d ", team_b_spawn_border.points[i].x, team_b_spawn_border.points[i].y);
	printf("\n");	
	
	printf("TEAM_A_TEMPLE_BORDER:  %d ", temple_a.boundary.num_of_points);
	for(i = 0; i<temple_a.boundary.num_of_points; i++)
		printf("%d %d ", temple_a.boundary.points[i].x, temple_a.boundary.points[i].y);
	printf("\n");
	
	printf("TEAM_B_TEMPLE_BORDER:  %d ", temple_b.boundary.num_of_points);
	for(i = 0; i<temple_b.boundary.num_of_points; i++)
		printf("%d %d ", temple_b.boundary.points[i].x, temple_b.boundary.points[i].y);
	printf("\n");
		
	printf("A_TEAM_TEMPLE_CENTER:  %d %d \n", temple_a.pos_x, temple_a.pos_y);
	printf("B_TEAM_TEMPLE_CENTER:  %d %d \n", temple_b.pos_x, temple_b.pos_y);
	
	printf("MAP:\n");
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++)
			printf("%c ", terrain[i][j]);
		printf("\n");	
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

path_t Map::get_shortest_path(point start, point goal){
	path_t ans;
	ans.last_path_index = -1;
	if(terrain[goal.y][goal.x] == 'J' || terrain[goal.y][goal.x] == 'A' || terrain[goal.y][goal.x] == 'B' ||
	   terrain[goal.y][goal.x] == 'W' || goal.x >= cols || goal.x < 0 || goal.y >= rows || goal.y < 0)
		return ans;

	int i, j;
	//Initialize Grid, Heuristic, Closed (i.e. visited), Action matrices
	char grid[75][75], heuristic[75][75], closed[75][75], action[75][75];
	
	for(i=0; i<rows;i++){
		for(j=0; j<cols; j++){
			heuristic[i][j] = (char)sqrt( pow(goal.x-j, 2) + pow(goal.y-i, 2));
			closed[i][j] = 0;
			action[i][j] = 0;
			grid[i][j] = 0;
			
			if(is_empty_location(j, i))
				grid[i][j] = 1;
		}
	}
	closed[start.y][start.x] = 1;
	action[start.y][start.x] = 'S';
	if(terrain[goal.y][goal.x] >= '1' && terrain[goal.y][goal.x] <= '9')
		grid[goal.y][goal.x] = 1;
	
	//Initialize Movements and associated cost
	point delta[8] = {  0, -1,	//Up
			    0,  1,	//Down
		 	   -1,  0,	//Left
			    1,  0,	//Right
			   -1, -1,	//Up-Left
			    1, -1,	//Up-right
			   -1,	1,	//Down-left
			    1,	1};	//Down-right
			   
	
	char delta_name[8] = {'v', '^', '>', '<', 'c', 'z', 'e', 'q'};
	int move_cost[8]  = {1, 1, 1, 1, 1, 1, 1, 1};

	//Kick start A* !
	priority_queue<op_item> openList;
	op_item a = {(int)heuristic[start.y][start.x], 0, start.x, start.y};		//[h, g, x, y]
	openList.push(a);
	
	bool Found = false;
	bool Resign = false;

	while( Found == false && Resign == false){
		if(openList.empty())
			Resign = true;
		else{
			op_item next = openList.top();
			
			int x = next.x;
			int y = next.y;
			int g = next.g;
		
			if(x == goal.x && y == goal.y)
				Found = true;
			else{
				for(i=0; i<(int)(sizeof(delta)/sizeof(point)); i++){
					int x2 = x + delta[i].x;
					int y2 = y + delta[i].y;
				
					if(x2 >= 0 && x2 < cols && y2 >= 0 && y2 < rows){
						if(closed[y2][x2] == 0 && grid[y2][x2] == 1){
							op_item b;
							b.g = g + move_cost[i];
							b.h = b.g + heuristic[y2][x2];
							b.x = x2;
							b.y = y2;
							openList.push(b);
							closed[y2][x2] = 1;
							action[y2][x2] = delta_name[i];
						}	
					}		
				}			
			}				
			openList.pop();
		}					
	}
	
	//Traceback and return ans
	if(Found == true){
		point t = goal;
		stack<point> s;
		s.push(t);
		char c;
		while(true){
			if(t.x == start.x && t.y == start.y)
				break;
			if(action[t.y][t.x] == '^')
				t.y = t.y-1;
			else if(action[t.y][t.x] == 'v')
				t.y = t.y+1;
			else if(action[t.y][t.x] == '<')
				t.x = t.x-1;
			else if(action[t.y][t.x] == '>')
				t.x = t.x+1;
			else if(action[t.y][t.x] == 'q'){
				t.x = t.x-1 ;
				t.y = t.y-1 ;
			}
			else if(action[t.y][t.x] == 'e'){
				t.x = t.x+1;
				t.y = t.y-1;
			}
			else if(action[t.y][t.x] == 'z'){
				t.x = t.x-1;
				t.y = t.y+1;
			}
			else if(action[t.y][t.x] == 'c'){
				t.x = t.x+1;
				t.y = t.y+1;
			}
			s.push(t);
		}
		while(!s.empty()){
			ans.last_path_index+=1;
			ans.path[ans.last_path_index][0] = (char)(s.top().x & 255);
			ans.path[ans.last_path_index][1] = (char)(s.top().y & 255);
			s.pop();
		}
	}
	
	return ans;	
}

/**
  * Modifies the location content in terrain at point t by c if location in empty.
**/  

void Map::modify_location_content(point t, char c){
	if(is_empty_location(t.x, t.y))
		terrain[t.y][t.x] = c;
}

/**
  * Retrives the location content in terrain at point t.
**/  

char Map::get_location_content(point t){
	if(t.x >=0 && t.x < cols && t.y >=0 && t.y < rows )
		return terrain[t.y][t.x];
	else
		return -1;	
}
/**
  * Returns euclidean distance between (x1, y1) and (x2, y2).
**/  
float Map::euclidean_dist(int x1, int y1, int x2, int y2){


}

float Map::euclidean_dist(point src, point dest){
	return euclidean_dist(src.x, src.y , dest.x, dest.y);
}

/**
  * Returns 1 if straight line (constucted using naive_line_drawaing_algo() Source: Wikipedia) 
  * between (x1, y1) and (x2, y2) has only '.'s. Else return 0;
**/  
bool Map::is_line_of_sight_clear(int x1, int y1, int x2, int y2){

}

bool Map::is_line_of_sight_clear(point src, point dest){
	return is_line_of_sight_clear(src.x, src.y , dest.x, dest.y);
}

/**
  * Returns true if terrain[y][x] can be occupied.
**/  

bool Map::is_empty_location(int x, int y){
	if(x >= cols || y >= rows || x < 0 || y < 0)
		return false;
		
	if(terrain[y][x] == 'J' || terrain[y][x] >= '1' && terrain[y][x] <= '9' || 
	   terrain[y][x] == 'A' || terrain[y][x] == 'B' || terrain[y][x] == 'W' ){
		return false;
	}
	else if(terrain[y][x] == '.' || terrain[y][x] == 'E' || terrain[y][x] == 'w' )
		return true;
	
	return false;		
}
