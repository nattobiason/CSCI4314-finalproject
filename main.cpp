
//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>
#include <math.h>
// #include "LTexture.hpp"
#include "colony.hpp"
# define pi           3.14159265358979323846





//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;




bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load Ant texture
	if( !gAntTexture.loadFromFile( "img/ant2.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	//Load food texture
	if( !gFoodTexture.loadFromFile( "img/food.png" ) )
	{
		printf( "Failed to load red texture!\n" );
		success = false;
	}

	//Load ant home texture
	if( !gHomeTexture.loadFromFile( "img/home.png" ) )
	{
		printf( "Failed to load green texture!\n" );
		success = false;
	}

	//Load pheromone texture
	if( !gHomeTrailTexture.loadFromFile( "img/home_pheromone.png" ) )
	{
		printf( "Failed to load blue texture!\n" );
		success = false;
	}

	//Load pheromone texture
	if( !gFoodTrailTexture.loadFromFile( "img/food_pheromone.png" ) )
	{
		printf( "Failed to load shimmer texture!\n" );
		success = false;
	}

	
	//Set texture transparency
	gAntTexture.setAlpha( 192 );
	gFoodTexture.setAlpha( 192 );
	gHomeTexture.setAlpha( 192 );
	gHomeTrailTexture.setAlpha( 192 );
	gFoodTrailTexture.setAlpha( 192 );
	
	return success;
}

void close(){
	//Free loaded images
	gAntTexture.free();
	gFoodTexture.free();
	gHomeTexture.free();
	gHomeTrailTexture.free();
	gFoodTrailTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] ){
	//Start up SDL and create window
	if( !init() ){
		printf( "Failed to initialize!\n" );
	}
	else{
		//Load media
		if( !loadMedia() ){
			printf( "Failed to load media!\n" );
		}
		else{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			Colony c(5);

			//While application is running
			while( !quit ){
				//Handle events on queue -- (while there exists an event to handle)
				while( SDL_PollEvent( &e ) != 0 ){
					//User requests quit
					if( e.type == SDL_QUIT ) quit = true;

				}
				//Move the dot
				c.move();

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render objects
				c.render();

				//Update screen
				SDL_RenderPresent( gRenderer );

			}
		}
	}

	//Free resources and close SDL
	close();



	return 0;
}


// https://lazyfoo.net/tutorials/SDL/04_key_presses/index.php