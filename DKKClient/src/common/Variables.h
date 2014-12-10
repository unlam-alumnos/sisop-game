/*
 * Variables.h
 *
 *  Created on: Nov 1, 2014
 *      Author: cristianmiranda
 */

#ifndef COMMON_VARIABLES_H_
#define COMMON_VARIABLES_H_

#include "../../../DKK/src/utils/includes/Graphics.h"

//The surfaces
extern SDL_Surface *screen;
extern SDL_Surface *background;
extern SDL_Surface *barriles;
extern SDL_Surface *DK;
extern SDL_Surface *mario;
extern SDL_Surface *luigi;
extern SDL_Surface *barril;
extern SDL_Surface *pauline;
extern SDL_Surface *fuego;
extern SDL_Surface *tarros;
extern SDL_Surface *player2name;
extern SDL_Surface *player1name;

extern SDL_Surface *message;


//The portions of the sprite map to be blitted
extern SDL_Rect DKclip[DKCANT];
extern SDL_Rect marioRclip[PLAYERCANT];
extern SDL_Rect luigiRclip[PLAYERCANT];
extern SDL_Rect marioLclip[PLAYERCANT];
extern SDL_Rect luigiLclip[PLAYERCANT];
extern SDL_Rect barrilSclip[BARRILSCANT];
extern SDL_Rect barrilFclip[BARRILFCANT];
extern SDL_Rect barrilesclip[BARRILESCANT];
extern SDL_Rect princesaRclip[PRINCESACANT];
extern SDL_Rect princesaLclip[PRINCESACANT];
extern SDL_Rect fuegoclip[FUEGOCANT];
extern SDL_Rect tarroclip[TARROCANT];

//The font that's going to be used
extern TTF_Font *font;

//The event structure that will be used
extern SDL_Event event;

extern SDL_Color textColor;


#endif /* COMMON_VARIABLES_H_ */
