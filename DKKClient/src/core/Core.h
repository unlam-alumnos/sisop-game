/*
 * Core.h
 *
 *  Created on: Oct 31, 2014
 *      Author: cristianmiranda
 */

#ifndef CORE_H_
#define CORE_H_

#include "../../../DKK/src/utils/includes/General.h"
#include "../../../DKK/src/utils/includes/Graphics.h"

#include "../common/Constants.h"
#include "../common/Variables.h"

SDL_Surface *load_image( std::string filename );
void clip_image(int w, int h, int cant, SDL_Rect clip[]);
bool init();
bool load_files();
void set_clips();
void clean_up();
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip );

#endif /* CORE_H_ */
