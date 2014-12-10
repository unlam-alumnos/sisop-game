/*
 * Core.cpp
 *
 *  Created on: Oct 31, 2014
 *      Author: cristianmiranda
 */

#include "Core.h"


//The surfaces
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *barriles = NULL;
SDL_Surface *DK = NULL;
SDL_Surface *mario = NULL;
SDL_Surface *luigi = NULL;
SDL_Surface *barril = NULL;
SDL_Surface *pauline = NULL;
SDL_Surface *fuego = NULL;
SDL_Surface *tarros = NULL;
SDL_Surface *player2name = NULL;
SDL_Surface *player1name = NULL;

SDL_Surface *message = NULL;


//The portions of the sprite map to be blitted
SDL_Rect DKclip[DKCANT];
SDL_Rect marioRclip[PLAYERCANT];
SDL_Rect luigiRclip[PLAYERCANT];
SDL_Rect marioLclip[PLAYERCANT];
SDL_Rect luigiLclip[PLAYERCANT];
SDL_Rect barrilSclip[BARRILSCANT];
SDL_Rect barrilFclip[BARRILFCANT];
SDL_Rect barrilesclip[BARRILESCANT];
SDL_Rect princesaRclip[PRINCESACANT];
SDL_Rect princesaLclip[PRINCESACANT];
SDL_Rect fuegoclip[FUEGOCANT];
SDL_Rect tarroclip[TARROCANT];

//The font that's going to be used
TTF_Font *font;

//The event structure that will be used
SDL_Event event;

SDL_Color textColor = {220, 0, 0};

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image using SDL_image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );

        //If the image was optimized just fine
		if( optimizedImage != NULL )
		{
			//Map the color key
			Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0, 0 );

			//Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
			SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
		}

    }

    //Return the optimized image
    return optimizedImage;
}

void clip_image(int xIni, int yIni, int w, int h, int cant, SDL_Rect clip[]){
	for (int i = 0 ; i < cant; i++ ){
		clip[i].x = xIni + i * w;
		clip[i].y = yIni;
		clip[i].w = w;
		clip[i].h = h;
	}
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Donkey Kong Tournament", NULL );

    //Initialize SDL_ttf
	if( TTF_Init() == -1 )
	{
		return false;
	}

	//Set the windHerow caption
	SDL_WM_SetCaption( "Donkey Kong Tournament", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
	//Open the font
	font = TTF_OpenFont( "gfx/font/font.ttf", 28 );

    //Load the background
    background = load_image( "gfx/images/background.png" );
    if( background == NULL )
    	return false;

    //Load the barriles
	barriles = load_image( "gfx/images/new/barriles26x40.png" );
	if( barriles == NULL )
		return false;

	//Load the DK
	DK = load_image( "gfx/images/new/DK52x38.png" );
	if( DK == NULL )
		return false;

	//Load the mario
	mario = load_image( "gfx/images/mario20x20.png" );
	if( mario == NULL )
		return false;


	//Load the luigi
	luigi = load_image( "gfx/images/luigi20x20.png" );
	if( luigi == NULL )
		return false;

	//Load the barril
	barril= load_image( "gfx/images/new/barril.png" );
	if( barril == NULL )
		return false;

	//Load the princesa
	pauline= load_image( "gfx/images/new/princesa16x18.png" );
	if( pauline == NULL )
		return false;

	//Load the fuego
	fuego= load_image( "gfx/images/new/fuego16x18.png" );
	if( fuego == NULL )
		return false;

	//Load the tarro
	tarros= load_image( "gfx/images/new/tarros20x38.png" );
	if( tarros == NULL )
		return false;


    //If everything loaded fine
    return true;
}

void set_clips()
{
    clip_image(0, 0 , DKW,DKH,DKCANT,DKclip);
    clip_image(0,PLAYERH, PLAYERW,PLAYERH,PLAYERCANT,marioRclip);
    clip_image(0,PLAYERH, PLAYERW,PLAYERH,PLAYERCANT,luigiRclip);
    clip_image(0,0,PLAYERW,PLAYERH,PLAYERCANT,marioLclip);
    clip_image(0,0,PLAYERW,PLAYERH,PLAYERCANT,luigiLclip);
    clip_image(0,0,BARRILW,BARRILH,BARRILSCANT,barrilSclip);
    clip_image(72,0,BARRILFW,BARRILH,BARRILFCANT,barrilFclip);
    clip_image(0,0,BARRILESW,BARRILESH,BARRILESCANT,barrilesclip);
    clip_image(0,0,PRINCESAW,PRINCESAH,PRINCESACANT,princesaRclip);
    clip_image(0,PRINCESAH,PRINCESAW,PRINCESAH,PRINCESACANT,princesaLclip);
    clip_image(0,0,FUEGOW,FUEGOH,FUEGOCANT,fuegoclip);
    clip_image(0,0,TARROW,TARROH,TARROCANT,tarroclip);
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( background );
    SDL_FreeSurface( barriles );
    SDL_FreeSurface( DK );
    SDL_FreeSurface( mario );
    SDL_FreeSurface( luigi );
    SDL_FreeSurface( barril );
    SDL_FreeSurface( pauline );
    SDL_FreeSurface( fuego );
    SDL_FreeSurface( tarros );
    SDL_FreeSurface( player1name );
    SDL_FreeSurface( player2name );

    //Close the font that was used
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Rectangle to hold the offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, clip, destination, &offset );
}

