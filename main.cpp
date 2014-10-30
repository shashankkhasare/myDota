#include <SDL/SDL.h>
#include<iostream>
using namespace std;

int running;
void pollAndServeEvents();

SDL_Surface * GetMenuSurface(char * ); 
int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_Surface *screen, *menu;
	screen = SDL_SetVideoMode(916, 663,32,SDL_SWSURFACE);
	
	menu= SDL_DisplayFormat(SDL_LoadBMP("images/menu.bmp"));
	
	
	SDL_Rect r;
	r.w = 916;
	r.h = 663;
	r.x = 0;
	r.y = 0;

	SDL_BlitSurface(menu, NULL, screen, &r);
	
	
	running = 1;
	while(running){
		Uint32 start = SDL_GetTicks();
		
		pollAndServeEvents();
		SDL_Flip(screen);
		
		if(1000/30 > SDL_GetTicks()-start)
			SDL_Delay(1000/30 - (SDL_GetTicks()-start));
	}
	
	SDL_Quit();



}

void pollAndServeEvents()
{
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

			case SDL_MOUSEBUTTONDOWN: 
				if ( e.button.button == SDL_BUTTON_LEFT){
					cout << "Left button click on  : (" << e.button.x << " , " << e.button.y << ")" << endl; 
				}else
				{
					cout << "Right button click on  : (" << e.button.x << " , " << e.button.y << ")" << endl; 
				}

		}
	}

}

SDL_Surface * GetMenuSurface(char * ){

