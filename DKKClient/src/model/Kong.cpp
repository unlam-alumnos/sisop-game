/*
 * Kong.cpp
 *
 *  Created on: Nov 4, 2014
 *      Author: cristianmiranda
 */

#include "Kong.h"

void Kong::move() {
	if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
		//throwRight();
	}

}

void Kong::throwLeft() {
	status = KONG_LEFT;
	//show(barril, i);
}

void Kong::throwRight() {
	status = KONG_RIGHT;
	//show(barril, i);
}



void Kong::show() {

	if(status == KONG_FRONT_1){
		cont++;
		if(cont == 8){
			frameH += 2;
			cont = 0;
		}
		if(frameH > 2){
			frameH = 0;
		}
	}else {
		cont++;
		switch(cont / 8){
			case 1:
				frameH = 1;
			break;
			case 2:
				frameH = 4;
			break;
			case 3:
				if (status == KONG_LEFT){
					frameH = 1;
				}else{
					//barril->init(i);
					frameH = 3;
				}
			break;
			case 4:
				status = KONG_FRONT_1;
				cont = 0;
			break;
		}
	}
	apply_surface( offSetx, offSety, DK, screen, &DKclip[ frameH ] );
}
