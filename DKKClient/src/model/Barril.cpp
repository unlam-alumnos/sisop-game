/*
 * Barril.cpp
 *
 *  Created on: Nov 3, 2014
 *      Author: cristianmiranda
 */

#include "Barril.h"



void Barril::init(int i, int direction) {
	if(direction == DIR_LEFT){
		offSetx = SCREEN_WIDTH / 2 - DKW / 2;
		offSety = FLOOR -BARRILH - 306;
	}
	barrels[i].w = width;
	barrels[i].h = height;
	barrels[i].x = offSetx;
	barrels[i].y = offSety;
	directionMultiPlier = direction;
	born();
}

void Barril::move(int i) {

	if(status != BARRIL_UNBORN && status != BARRIL_BORN){
		//Keep the stick figure in bounds
		if( offSety <= FLOOR - 36){ // Si no es planta baja...
			if( offSetx < 0 ) {
				offSetx = 0;
				directionMultiPlier = 1;
			} else if ( offSetx > SCREEN_WIDTH - width) {
				offSetx = SCREEN_WIDTH - width;
				directionMultiPlier = -1;
			}
		}

		inALadder = checkLadders();
		// Si me encuentro en movimiento horizontal y a su vez estoy por bajar una escalera
		// le doy un poco mas de velocidad para que el barril quede mas centrado
		if(inALadder == LADDER_TOP && velocityX != 0){
			velocityX = 4 * directionMultiPlier;
			offSetx += velocityX;
		}
		if(inALadder != NO_LADDER && inALadder != LADDER_BOTTOM){
			velocityY = 4;
			offSety += velocityY;
		} else {
			velocityX = 4 * directionMultiPlier;
			offSetx += velocityX;
		}

		int wall = checkWalls();
		// Si el barril está en el aire (3 pixeles arriba de un bloque se considera aire, menos no)
		// Lo muevo verticalmente hacia abajo de a 7 pixeles (gravedad)
		if((abs((walls[wall].y - height) - offSety)) > 3){
			if(inALadder == NO_LADDER){ // Para que no se caiga de la escalera cuando no se mueve verticalmente
				offSety += 7;
			}
		} else {
			// Hago que el barril se mantenga sobre el bloque
			offSety = walls[wall].y - height;
		}

		barrels[i].x = offSetx;
		barrels[i].y = offSety;

		if( (offSetx == 0 || offSetx == SCREEN_WIDTH - PLAYERW) && offSety > FLOOR - 36){
			die();
		}

	}
}

void Barril::show(int i) {

	if(status != BARRIL_UNBORN && status != BARRIL_BORN){

		//If Foo is moving left
		if(velocityX != 0){
			//Set the animation to left
			status = BARRIL_SIDE_1;

			//Move to the next frame in the animation
			frameH++;
		}

		//Loop the animation
		if( frameH >= BARRIL_SIDE_4)
		{
			frameH = BARRIL_SIDE_1;
		}

		apply_surface( offSetx, offSety, barril, screen, &barrilSclip[ frameH ] );

		if(checkLadders()) {
			//If Foo is moving left
			if( (velocityY > 0 && inALadder == LADDER_TOP) ||
				(velocityY < 0 && inALadder == LADDER_BOTTOM) ||
				(velocityY != 0 && inALadder == LADDER_MIDDLE))
			{
				//Set the animation to left
				status = BARRIL_FRONT_1;

				//Move to the next frameH in the animation
				frameV++;
			}
			//If Foo standing
			else
			{
				//Restart the animation
				frameV = 0;
			}

			//Loop the animation
			if( frameV >= 1 )
			{
				frameV = 0;
			}

			//Show the stick figure
			if( status == BARRIL_FRONT_1 || status == BARRIL_FRONT_2)
			{
				apply_surface( offSetx, offSety, barril, screen, &barrilFclip[ frameV ] );
			}
		}
	} else {
		if (status == BARRIL_BORN){
			cont ++;
			if(cont == 20){
				cont = 0;
				status = BARRIL_SIDE_1;
			}
		}
	}
}

void Barril::born() {
	status = BARRIL_BORN;
}

void Barril::die() {
	status = BARRIL_UNBORN;
	offSetx = SCREEN_WIDTH / 2 + DKW / 2;
	offSety = FLOOR -BARRILH - 306;
	directionMultiPlier = DIR_RIGHT;
}
