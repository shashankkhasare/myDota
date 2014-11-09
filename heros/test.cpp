#include "hero.h"
#include <iostream>
#include<unistd.h>

int main(){
	int i;
	point curr;
	curr.x = 12, curr.y = 34;
	Hero h(0, 'P', TEAM_A, curr);

	cout <<"hid: "<<h.hid<< endl;
	cout <<"symbol_on_map: "<<h.symbol_on_map << endl;
	cout <<"power.name: "<<h.mpower.name << endl;
	cout <<"power.magic: "<<h.mpower.magic << endl;
	cout <<"power.aspeed: "<<h.mpower.attack_speed << endl;
	cout <<"power.mspeed: "<<h.mpower.movement_speed << endl;
	cout <<"power.damage: "<<h.mpower.damage << endl;
	cout <<"power.time: "<<h.mpower.time << endl;
	cout <<"power.range: "<<h.mpower.range << endl;
	cout <<"power.targets: "<<h.target_id<<h.target_type << endl;
	cout <<"isMovementDisabled: "<<h.isMovementDisabled << endl;
	cout <<"isMeleeDisabled: "<<h.isMeleeDisabled << endl;
	cout <<"health: "<<h.health << endl;
	cout <<"max_health: "<<h.max_health << endl;
	cout <<"movement_speed: "<<h.movement_speed << endl;
	cout <<"attack_speed: "<<h.attack_speed << endl;
	cout <<"attack_mode: "<<h.attack_mode << endl;
	cout <<"damage: "<<h.damage << endl;
	cout <<"bagcp: "<<h.bag.bag_capacity << endl;
	cout <<"bagccp: "<<h.bag.curr_capacity << endl;

	for(i=0; i<7;i++){
		cout <<h.bag.items[i].image_file << endl;
		cout <<h.bag.items[i].movement_speed << endl;
		cout <<h.bag.items[i].attack_speed << endl;
		cout <<h.bag.items[i].health << endl;
		cout <<h.bag.items[i].damage_melee << endl;
		cout <<h.bag.items[i].damage_magic << endl;
		cout <<h.bag.items[i].temp_heal << endl;
		cout <<h.bag.items[i].required_space << endl;
		cout <<h.bag.items[i].quantity << endl;
	}
		
	cout <<"state: "<<h.state << endl;
	
	return 0;
}
