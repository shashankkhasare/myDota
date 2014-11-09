#include "spell.h"

spell init_spell(const char *filename){
	spell i;
	int j;
	ifstream infile(filename);
	
	if(!infile.good()){
		cout << "Magic Spell file " << filename << " doesn't exist. Exiting.\n";
		exit(0);
	}
	
	string token;
	while(infile >> token){
		if(token == "NAME:")
			infile >> i.name;
		else if(token == "MAGIC:")
			infile >> i.magic;
		else if(token == "ATTACK_SPEED:")
			infile >> i.attack_speed;
		else if(token == "MOVEMENT_SPEED:")
			infile >> i.movement_speed;
		else if(token == "DAMAGE:")
			infile >> i.damage;
		else if(token == "TIME:")
			infile >> i.time;
		else if(token == "RANGE:")
			infile >> i.range;
	}
	infile.close();	

	i.is_Disabled = false;
	for(j = 0; j<CAPACITY_PER_SPELL; j++){
		i.path[j].last_path_index = -1;	
		i.index[j] = -1;
	}
	
	i.nitro = CAPACITY_PER_SPELL;	
	return i;
}
