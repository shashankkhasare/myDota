#include "map.h"

int main(){
	Map m("m1");
	
	point start = {10, 74}, goal = {64 , 0}, t;
	printf("%c\n\n", m.get_location_content(goal));
	path_t ans = m.get_shortest_path(start, goal);
	int i;
	for(i=0; i<=ans.last_path_index; i++){
		t = {(int)ans.path[i][0], (int)ans.path[i][1]};
		m.modify_location_content(t,'P');
	}	
	m.PrintMap();
}
