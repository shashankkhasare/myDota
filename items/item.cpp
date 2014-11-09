#include "item.h"

item init_item(const char *filename){
	item i;
	ifstream infile(filename);
	
	if(!infile.good()){
		cout << "Item file " << filename << " doesn't exist. Exiting.\n";
		exit(0);
	}
	
	string token;
	while(infile >> token){
		if(token == "IMAGE_FILE:")
			infile >> i.image_file;
		else if(token == "MOVEMENT_SPEED:")
			infile >> i.movement_speed;
		else if(token == "ATTACK_SPEED:")
			infile >> i.attack_speed;
		else if(token == "HEALTH:")
			infile >> i.health;
		else if(token == "DAMAGE_MELEE:")
			infile >> i.damage_melee;
		else if(token == "DAMAGE_MAGIC:")
			infile >> i.damage_magic;
		else if(token == "TEMPLE_HEAL:")
			infile >> i.temp_heal;
		else if(token == "REQUIRED_SPACE:")
			infile >> i.required_space;
	}
	i.quantity = 0;
	infile.close();	
	
	return i;
}
