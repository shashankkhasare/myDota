class Widget{

	SDL_Rect r; 
	SDL_Surface * image, *image_hover = NULL;
	bool mouseOver = false;

public : 
	void setLocation(int x , int y ) {

		r.x = x; 
		r.y = y ; 
	}

	void setDim(int w, int h){
		r.w = w; 
		r.h = h ; 
	}
	/*
	SDL_Rect getRect()
	{
		return r; 
	}*/
	SDL_Rect *  getRectAddr()
	{
		return &r; 
	}

	SDL_Surface * getSurface(){
		if(mouseOver){
			return image_hover;
		}
		else
			return image; 
	}
	Widget(char *img, char *img_hover){
		image = SDL_DisplayFormat(SDL_LoadBMP(img));
		if ( img_hover != NULL)
			image_hover = SDL_DisplayFormat(SDL_LoadBMP(img_hover));
	}

	Widget(char *img,char * img_hover, int x, int y, int w, int h){
		image = SDL_DisplayFormat(SDL_LoadBMP(img));
		if ( img_hover != NULL)
			image_hover = SDL_DisplayFormat(SDL_LoadBMP(img_hover));
		r.x = x; 
		r.y = y ; 
		r.w = w; 
		r.h = h ; 
	}
	void deleteWhite(SDL_Surface * screen)
	{

		SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	
		if ( image_hover != NULL)
			SDL_SetColorKey(image_hover, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	

	}

	~Widget(){
		SDL_FreeSurface(image);
		if ( image_hover != NULL)
			SDL_FreeSurface(image_hover);
	}
	void handleHover(SDL_Event e ) {
		int x, y ; 
		x = e.motion.x; 
		y = e.motion.y; 
		if( ( x > r.x ) && ( x < r.x + r.w ) && ( y > r.y ) && ( y < r.y + r.h ) ) { 
			mouseOver = true;
		} 
		else
		{
			mouseOver = false;
		} 
	}
	bool isInRange(int x, int y){
		if( ( x > r.x ) && ( x < r.x + r.w ) && ( y > r.y ) && ( y < r.y + r.h ) ) { 
			return true; 
		} 
		else
		{
			return false;
		} 
	}


};
