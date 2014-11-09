#include "hero.h"
#include "../magic_spells/spell.h"
#include "../maps/map.h"

extern requests message[7];

Hero::Hero(){}

Hero::Hero(int h_id, char symbol, point curr, int team){
	ifstream infile;
	string t, token;
	
	hid = h_id;
	symbol_on_map = symbol;
	teamid = team;
	target_id = -1, target_type = -1;
	isMovementDisabled = false;
	isMeleeDisabled = false;
	
	attack_mode = MELEE;
	
	curr_pos = curr;
	dest_pos.x = -1, dest_pos.y = -1;
	
	state = STEADY;
	path.last_path_index = -1;
	curr_path_index = -1;	

	if(h_id == 0)
		t = "heros/h1";
	else if(h_id == 1)
		t = "heros/h2";
	else if(h_id == 2)
		t = "heros/h3";
	else if(h_id == 3)
		t = "heros/h4";
	else{
		cout << "Invalid hid: " << h_id << ". Exiting.\n";
		exit(0);
	}
	
	infile.open(t.c_str());
	if(!infile.good()){
		cout << "Hero file " << t << " doesn't exist. Exiting.\n";
		exit(0);
	}
	
	while(infile >> token){
		if(token == "MPOWER_ID:"){
			infile >> token;
			t = "magic_spells/"+token;
			mpower = init_spell(t.c_str());
		}
		else if(token == "MAX_HEALTH:"){
			infile >> max_health;
			health = max_health;
		}
		else if(token == "MOVEMENT_SPEED:"){
			infile >> movement_speed;
		}
		else if(token == "ATTACK_SPEED:"){
			infile >> attack_speed;
		}
		else if(token == "DAMAGE:"){
			infile >> damage;
		}
		else if(token == "ITEM_BAG_CAPACITY:"){
			infile >> bag.bag_capacity;
			bag.curr_capacity = 0;
			int i;
			for(i=1; i<=NUM_OF_ITEMS; i++){
				t = "items/i";
				t = t+to_string(i);
				bag.items[i-1] = init_item(t.c_str());
			}
		}
		
		
	}
}

void Hero::process_dfa(){
	if(inst.empty())
		return;
	
	if(inst.front().command == CMD_GOTO_X_Y){
		if(!isMovementDisabled){
			dest_pos.x = inst.front().x;
			dest_pos.y = inst.front().y;
			state = MOVING;
			message[4].valid = true;
		}
		inst.pop();
		return;
	}
	
	if(inst.front().command == CMD_ATTACK_PID){
		if(inst.front().attack_mode == MELEE && !isMeleeDisabled || inst.front().attack_mode == MAGIC && !mpower.is_Disabled){
			attack_mode = inst.front().attack_mode;
			target_id = inst.front().pid;
			if(target_id >= 0 && target_id <= 3 ){
				target_type = TARGET_TYPE_HERO;
				message[3].valid = true;
				state = MOVING_TO_ATTACK_HERO;
			}
			else{
				target_type = TARGET_TYPE_TEMPLE;
				message[2].valid = true;
				state = MOVING_TO_ATTACK_TEMPLE;
			}	
			
		}
		inst.pop();
		return;			
	}
	
	if(inst.front().command == CMD_USEITEM_X){
		int item_to_consume = inst.front().x-1;
		if(bag.items[item_to_consume].quantity >= 1){
			if(item_to_consume != 6){	//!Temple Healer
				movement_speed += bag.items[item_to_consume].movement_speed;
				attack_speed += bag.items[item_to_consume].attack_speed;
				health += bag.items[item_to_consume].health;
				
				if(health > max_health)
					health = max_health;
				
				damage += bag.items[item_to_consume].damage_melee;
				mpower.damage += bag.items[item_to_consume].damage_magic;
			}
			else{
				message[0].valid = true;
				message[0].int1 = teamid;
				message[0].int2 = bag.items[item_to_consume].temp_heal;
			}
				
			bag.curr_capacity += bag.items[item_to_consume].required_space;
			bag.items[item_to_consume].quantity -= 1;
		}
		inst.pop();
		return;
	}
	
	if(inst.front().command == CMD_GRABITEM_X_Y){
		if(!isMovementDisabled){
			state = MOVING_TO_COLLECT;
			message[5].valid = true;
			message[5].int1 = inst.front().x;
			message[5].int2 = inst.front().y;
		}
		inst.pop();
		return;
	}	
}

void Hero::drive(){
	if (state == STEADY || state == MOVING || state == MOVING_TO_COLLECT){
		if(state == MOVING || state == MOVING_TO_COLLECT)
			message[6].valid = true;
	}
	else if( state == MOVING_TO_ATTACK_TEMPLE || state == MOVING_TO_ATTACK_HERO){
		if(attack_mode == MELEE && curr_path_index == path.last_path_index-1){
			if(!isMeleeDisabled){
				int z;
				if(target_type == TARGET_TYPE_TEMPLE)
					z = 0;
				else
					z = 1;
				
				message[z].valid = true;
				message[z].int1 = target_id;
				message[z].int2 = -(damage*attack_speed);
			}
		}
		
		if(attack_mode == MELEE && curr_path_index < path.last_path_index-1)
			message[6].valid = true;
	}
	process_dfa();
}

void Hero::spawn(Map *m){
	temple *l;
	if(teamid == TEAM_A)
		l = &(m->temple_a);
	else
		l = &(m->temple_b);
	
	int c,v,b;
	for(c = 0; c < l->boundary.num_of_points; c++){
		for(v = -1; v<= 1; v++){
			for(b = -1; b<=1; b++){
				int x, y;
				x = (int)(l->boundary.points[c].x);
				y = (int)(l->boundary.points[c].y);
				if(m->is_empty_location(x+b, y+v)){
					curr_pos.x = x+b;
					curr_pos.y = y+v;
					dest_pos.x = -1;
					dest_pos.y = -1;
					state = STEADY;
					attack_mode = MELEE;
					path.last_path_index = -1;
					health = max_health;
					return;
				}
			}
		}
	}
}

void Hero::route(Map *m, point dest){
	char temp = m->terrain[dest.y][dest.x];
	
	m->terrain[curr_pos.y][curr_pos.x] = '.';
	m->terrain[dest.y][dest.x] = '.';
	
	path_t p = m->get_shortest_path(curr_pos, dest);
	if(p.last_path_index != -1)
		path = p;
	else{
		curr_path_index = -1;
		path.last_path_index = -1;
		dest_pos.x = -1;
		dest_pos.y = -1;
		state = STEADY;
		attack_mode = MELEE;
	}
	m->terrain[curr_pos.y][curr_pos.x] = symbol_on_map;
	m->terrain[dest.y][dest.x] = temp;
}
