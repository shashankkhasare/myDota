#include <SDL/SDL.h>
#include<iostream>
#include "Widget.h"
using namespace std;

int running;
void pollAndServeEvents();

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface *screen;
	screen = SDL_SetVideoMode(916, 663,32,SDL_SWSURFACE);

		
	

	Widget menu("images/menu.bmp", NULL);
	menu.setLocation(0, 0);
	menu.setDim(916, 663);


	//adding the menu background 
	SDL_BlitSurface(menu.getSurface(), NULL, screen, menu.getRectAddr());

	// instantiate single player 
	Widget singlePlayerButton("images/single_player.bmp", "images/single_player_hover.bmp", 350, 270 , 195, 51);
	singlePlayerButton.deleteWhite(screen);

	// instantiate multiplayer 
	Widget multiPlayerButton("images/multiplayer.bmp", "images/multiplayer_hover.bmp", 350, 330 , 195, 51);
	multiPlayerButton.deleteWhite(screen);

	// instantiate exit
	Widget exitButton("images/exit.bmp", "images/exit_hover.bmp", 350, 390 , 195, 51);
	exitButton.deleteWhite(screen);

	running = 1;
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

				case SDL_MOUSEBUTTONDOWN: 
					if ( e.button.button == SDL_BUTTON_LEFT){
						if ( exitButton.isInRange(e.button.x, e.button.y ))
							running = 0 ; 
						else if ( singlePlayerButton.isInRange(e.button.x, e.button.y )){
							cout << "Single Player Selected  " << endl ; 
							// TODO : start the server in bot mode 
						}
						else if ( multiPlayerButton.isInRange(e.button.x, e.button.y )){
							// TODO : start server in network mode 
							cout << "Multi Player network mode  Selected  " << endl ; 
						}
					}else
					{
						cout << "Right button click on  : (" << e.button.x << " , " << e.button.y << ")" << endl; 
					}
					break;
				case SDL_MOUSEMOTION:
					singlePlayerButton.handleHover(e);
					multiPlayerButton.handleHover(e);
					exitButton.handleHover(e);
					break;

			}
		}
		SDL_BlitSurface(singlePlayerButton.getSurface(), NULL, screen, singlePlayerButton.getRectAddr());
		SDL_BlitSurface(multiPlayerButton.getSurface(), NULL, screen, multiPlayerButton.getRectAddr());
		SDL_BlitSurface(exitButton.getSurface(), NULL, screen, exitButton.getRectAddr());
		SDL_Flip(screen);

		if(1000/30 > SDL_GetTicks()-start)
			SDL_Delay(1000/30 - (SDL_GetTicks()-start));
	}

	SDL_Quit();



}


