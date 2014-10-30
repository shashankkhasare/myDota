#include <SDL/SDL.h>
int main(){
	char map[75][75] = {
"wwwwwwwwwwwwW....J...JJ....J.J........J....8........2....3.......BBBBBBBBBB",
"wwwwwwwwwwJwwWW.J.......................J........JJ..............BBBBBBBBBB",
"wwwwwwwwwwwwwWW......J....J................................J.....BBBBBBBBBB",
"wwwwwwwwwwwwJJwWW...............9.J...7...J......4................BBBBBBBBB",
"wwwwwwwwwwJwJwwWWJ...................J...................J........BBBBBBBBB",
"wwwwJww6wwwwwwwwwWW..JJ....J.........................J.........2.5.BBBBBBBB",
"wwwwwwwwwwwwwwwwJWW..............J..............7.4...........J....BBBBBBBB",
"wwwwwwwwwwwwwwwwwwwW....................................9.......J...BBBBBBB",
"wwJwwwwwwwwwwwwwwwww............J.................................J.6.BBBBB",
"wwwwwwwwwwwwwwwwwwwwwW.J..........................JJ....................BBB",
"wwwwwwwwwwwwwwwwwwwwwwW.7....J.............J.J.........8.J7......J.....J.J.",
"wwwwwwww6wwwwwJwJwwwwwwW5.............J....J...............................",
"WwwwwwwwwwwwwwwwwwwwwwwwW........J.......................J..........8.J....",
".WwwwwwwwwwwwwwJ7wwwwwwwwW............J.........J..................J.......",
"..WwwJwwwwwwwwwwwwwwwwwwwwW.......J........J..J...J..........J...4.........",
"..JWwJwwwwJwww5wwJwwwwwwwwwW....J.........5.......J..J....J................",
"....WwwwwwJwwwwwwwwwJwww4ww4W............2..........J5......J....J.J....J..",
".....WwwwwwwwwwwwwwwwwwwwwwwwW...........J..5..........................JJ..",
"......WwwwJwwwwwwwwwwJJwwwwJww...J.........................................",
".......WwwwwwwwwwwwwwwwwwwJwwJwWJ..5.J......6..............................",
".........wwwwwwwwwwwwwwwwwwwwwwwW....J................J....4...............",
".........WwwwwwwwwwwwwwwwaaawwwwwW..........JJ.........................9...",
"J.JJJ.....WwwwwwwwwwwwwwaaaaawwwwwW.....................J..................",
"...6....5..WwJwwwwwwwwwaaaaaaawwwwwW.............J...............J...9JJ...",
".J.......J8.WwwwwwwwwwaaaaaaaaawwwwwW....6..7.................J....J.......",
".4...........WwwwwwwwwaaaaaaaaawwwwJwW........J...J.............5........J.",
"......J.......WwwwwwJ4aaaaaaaaawwwwwwwW...8...J........J............J......",
"...............Wwwwww8JaaaaaaawwwwwwwwwW.........J.........J....3...J......",
".J5.............Wwwwwwwwaaaaawwwwwwwwwww...................................",
".......J9.45..2..WwwwwwwwaaawwwwwJwwwwwwwW............J5...........J.......",
"....6..............wwwwwww2wwwwwwwwwJwwwwwW..................7.............",
".J.................WwwwwwwwwwwwwwwwwwwwwwwwWJ.......J......................",
"....................WwwwwwwwwwwwwwwwwwwwwwwwW..............................",
"........J.......5....Wwwwwwwwwww7wwwwwwwwwwwwW....J.............3JJ...6....",
"J.............J.......WwwwwwwwwwwwwJJJwwwwwwwwW............................",
"..1...................JWwwwwwJwwwwwwJwwwwwwwwwwW..J.....J.............1....",
"...J..........J.........Wwwwwwwww1wwwwwwwwwwwwwwW.........J................",
"......5..J.......J.......WwwwwwwwwwwwwwwwwwwwwwwwW..........8..J...........",
"....J.....J...............Www5w3wwwwwwwwwwwwwwwww7.....7.9.................",
"3....7...J............J....WwwwwwwwwwwwwwwwwwwwwwwwW.......J...............",
"J..............J..J.J.J..2...wJwwwwww7wwwwwwwwwwwwwwW.........J............",
".4.J7....3...................WwwwwwJwwwwwwwww4wwwwwwwW...5....J......3.....",
"................J.............WwwwwwwwwwwwwwwJwwwwwwwwW.................J..",
"..J..............JJ............WJwwJJwwwwwww4wwwwwwJwwwW.............J.....",
"...........J....................WwwJwwwwwwwwwwwwwwwwww1wW.............8....",
"......5.J.J.............J4..J....WwwwwwwwwwwwwwwwbbbwwwwwW.................",
"......J........J.....J.........7..WwwwwwwwwwwwwwbbbbbwwwwwW................",
"J..........................J.......WwwwwwwwwwwwbbbbbbbwwwwwW...............",
"..................J...J........J....Wwwwwwwwwwbbbbbbbbbwwwww............J..",
".............J.......J...............W4wwwwwwwbbbbbbbbbwwwwwwW.............",
"........J...............J.......J..J...wJwwwwwbbbbbbbbbwwwwwwwWJ...........",
"....................3..................WwwJwwwwbbbbbbbwwwwwww6wW...........",
".......................6..J...........J.Wwww9wwwbbbbbwwwwwwwwwwwW..........",
"......8.................J................WwwwwwwwbbbwwJwwwJwwwww7W.........",
"..........................J...............WwwwJwwwwwwwwwwwwJwwwwwwW.J......",
"........J................................J.WwwwwwwwwwwwwwwwJJww4wwwW.......",
"...J.............................6..........WwwwwwwwwwwwwwJwwwwwwwwwW......",
"...............J.......8....................JWwwwwwwwwwwwwJwwwJwwwwwwW..J..",
"......................J......J................WwwwwwwJwwwwwwwwwwwwwwww.....",
".J.............JJ..J................J..J.......WwwwwwJwwwJwwwwwwwwwwwwwW...",
"J........J......................J................wwwww8wwwwwwwwwwwwwwwwwW..",
"....J.............J...........J...........J....J.Www7wwwwwwwwwwwwwwwwwwwwW.",
".J......J........J....3..J...............J...7....WwwwwwwwwwwwwwwwwwwwwwwwW",
"................................J............4.....WwwwwwwwwwwJwwwwwwwJwwww",
"........J..............J...........................JWwwwJwJwwwwwwwwwJwwwwww",
"AAA.............J..7..J..............................Wwwwwwwwwwwwwwwwwwwwww",
"AAAAA....J.........................J.8......7.........Wwwwwwwwwwwwwwwwwwwww",
"AAAAAAA.........J...................J..................Wwwwwwwwwwwwwwww8www",
"AAAAAAAA.9......J..................................J....Wwwwwwwwwwwwwwwwwww",
"AAAAAAAA.......5....8.............J......................Wwwwwwwwwwwwwwwwww",
"AAAAAAAAA...............J........J..............J..........wwwwwwwwwwww3www",
"AAAAAAAAA.J.......J......7.4..........J6.J.9........J......WJwJwwwwwwwJwwww",
"AAAAAAAAAA....................J.....................J..J....WwJwwwwJwwwwwww",
"AAAAAAAAAA............J...4......9.......J....J..............WwwwwwJwwwwwww",
"AAAAAAAAAA....................................................Wwwwwwwwwwwww"};
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_Surface *screen, *fw, *tw, *w, *t, *it;
	screen = SDL_SetVideoMode(750,750,32,SDL_FULLSCREEN);
	
	fw = SDL_DisplayFormat(SDL_LoadBMP("wfree750.bmp"));  //Required for transparnecy
	tw = SDL_DisplayFormat(SDL_LoadBMP("wtree.bmp"));
	w = SDL_DisplayFormat(SDL_LoadBMP("btree15_c.bmp"));
	t = SDL_DisplayFormat(SDL_LoadBMP("Temple1.bmp"));
	it = SDL_DisplayFormat(SDL_LoadBMP("items.bmp"));
	
	
	SDL_SetColorKey(tw, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	
	SDL_SetColorKey(w, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	
	SDL_SetColorKey(t, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	
	
	//Background Grass	
	int i, j;
	SDL_Rect r;
	r.w = 750;
	r.h = 750;
	r.x = 0;
	r.y = 0;
	SDL_BlitSurface(fw, NULL, screen, &r);
	
	//Temples
	r.w = 90;
	r.h = 90;
	r.x = 22*10;
	r.y = 21*10;
	SDL_BlitSurface(t, NULL, screen, &r);
	r.x = 46*10;
	r.y = 45*10;
	SDL_BlitSurface(t, NULL, screen, &r);	
	
	//Items
	SDL_Rect item[9];
	for(i=0;i<9;i++){
		item[i].x = 10*i;
		item[i].y = 0;
		item[i].w = 10;
		item[i].h = 10;
	}
	for(i=0;i<75;i++)
		for(j=0;j<75;j++)
			if(map[i][j] >= '1' && map[i][j] <= '9'){
				r.x = 10*j;
				r.y = 10*i;
				SDL_BlitSurface(it, &item[map[i][j]-'1'], screen, &r);
			}
	
	//Jungle
	r.w = 15;
	r.h = 15;
	for(i=0; i<75; i++){
		for(j = 0; j<75; j++){
			if(map[i][j] == 'J'){
				r.x = 10*j;
				r.y = 10*i;
				SDL_BlitSurface(tw, NULL, screen, &r);
			}
			else if	(map[i][j] == 'W'){
				r.x = 10*j;
				r.y = 10*i;
				SDL_BlitSurface(w, NULL, screen, &r);
			}
		}	
	}		
	
	int running = 1;
	while(running){
		Uint32 start = SDL_GetTicks();
		SDL_Event e;
		
		while(SDL_PollEvent(&e)){
			switch(e.type){
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					if(e.key.keysym.sym == SDLK_ESCAPE)
						running = 0;
					break;	
			}
		}
		SDL_Flip(screen);
		
		if(1000/30 > SDL_GetTicks()-start)
			SDL_Delay(1000/30 - (SDL_GetTicks()-start));
	}
	
	SDL_FreeSurface(fw);
	SDL_FreeSurface(tw);
	SDL_FreeSurface(w);
	SDL_FreeSurface(t);
	SDL_Quit();
}
