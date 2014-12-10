/*
 * Fuego.cpp
 *
 *  Created on: 7/11/2014
 *      Author: her
 */

#include "Fuego.h"


void Fuego::init(int i, int direction) {
	if(direction == DIR_LEFT){
		offSetx = SCREEN_WIDTH / 2 - TARROW / 2;
		offSety = FLOOR - FUEGOH;
	}
	flames[i].w = width;
	flames[i].h = height;
	flames[i].x = offSetx;
	flames[i].y = offSety;
	directionMultiPlier = direction;
	born();
}

void Fuego::move(int i) {

	if(status != FUEGO_UNBORN ){
		//Keep the stick figure in bounds

		if( offSetx < 0 ) {
			offSetx = 0;
			directionMultiPlier = 1;
		} else if ( offSetx > SCREEN_WIDTH - width) {
			offSetx = SCREEN_WIDTH - width;
			directionMultiPlier = -1;
		}


		inALadder = checkLadders();
		// Si me encuentro en movimiento horizontal y a su vez estoy por bajar una escalera
		// le doy un poco mas de velocidad para que el barril quede mas centrado
		if(inALadder == LADDER_BOTTOM && velocityX != 0){
			velocityX = 4 * directionMultiPlier;
			offSetx += velocityX;
		}
		if(inALadder != NO_LADDER && inALadder != LADDER_TOP){
			velocityY = -4;
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

		flames[i].x = offSetx;
		flames[i].y = offSety;

		if( (offSetx == SCREEN_WIDTH - FUEGOW|| offSetx == 0 ) && offSety < 200 )
		{
			die();
		}

	}
}

void Fuego::show(int i) {

	if(status != FUEGO_UNBORN){
		cont++;
		//If Foo is moving left
		if(velocityX != 0){

			//Move to the next frame in the animation
			if(cont % 2 == 0)
				frameH++;
		}

		//Loop the animation
		if( frameH >= FUEGOCANT)
		{
			frameH = 0;
			cont = 0;
		}
		apply_surface( offSetx, offSety, fuego, screen, &fuegoclip[ frameH ] );
	}
}

void Fuego::born() {
	status = FUEGO_BORN;
}

void Fuego::die(){
	status = FUEGO_UNBORN;
	offSetx = SCREEN_WIDTH / 2;
	offSety = FLOOR - FUEGOH;
	directionMultiPlier = DIR_RIGHT;

}
