class Entity{

	SDL_Rect r; 

	SDL_Rect fill_health; 

	SDL_Rect fill_nitro;

	SDL_Surface * canvas;
	SDL_Surface * screen;
	char * image; 

	bool mouseOver = false;
	int health, nitro; 

	public : 
	void setLocation(int x , int y ) {

		r.x = x; 
		r.y = y ; 

		fill_health.x = x; 
		fill_health.y = y; 

		fill_nitro.x = x; 
		fill_nitro.y = y + 3 ; 
	}

	void setDim(int w, int h){
		r.w = w; 
		r.h = h ; 


	}
	SDL_Rect *  getRectAddr()
	{
		return &r; 
	}

	SDL_Surface * getSurface(){
		int green=(int)( 0xff * health )/ 100.0 ;
		int red = 0xff - green; 
		Uint32 color = SDL_MapRGB(screen -> format, red ,  green, 0);

		fill_health.w  = (int) ( r.w * health ) / 100.; 
		SDL_FillRect(screen, &fill_health, color); 


		green = (int ) ( 0xff * nitro )  / 100.0 ; 
		red = 0xff - green ; 
		color = SDL_MapRGB(screen-> format , red, green, 0) ; 
		fill_nitro.w = ( int) (r.w * nitro ) / 100; 
		SDL_FillRect(screen, &fill_nitro, color); 

		SDL_SetColorKey(canvas , SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	
		return canvas; 
	}

	Entity(char *img){
		image = img; 
	}
	Entity(){}


	Entity(char *img, int x, int y, int w, int h){
		image = img; 
		r.x = x; 
		r.y = y ; 
		fill_health.x = x; 
		fill_health.y = y; 

		fill_nitro.x = x; 
		fill_nitro.y = y + 3 ; 
		r.w = w; 
		r.h = h ; 

		fill_health.h = 2; 

		fill_nitro.h = 2; 
	}
	void load_image(){
		canvas = SDL_DisplayFormat(SDL_LoadBMP(image)); 
	}

	void setImage(char * i) {
		image = i  ;
	}

	void setBGScreen(SDL_Surface * scr)
	{
		screen = scr; 
	}
	void setHealth( int h ) 
	{
		health = h ; 
	}
	void setNitro( int h ) 
	{
		nitro = h; 
	}

	~Entity(){
		SDL_FreeSurface(canvas);
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
