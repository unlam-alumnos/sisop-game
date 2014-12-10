/*
 * Tarro.cpp
 *
 *  Created on: 5/11/2014
 *      Author: her
 */

#include "Tarro.h"


void Tarro::move() {


}

void Tarro::show() {
	cont++;
	if(cont == 4){
		frameH++;
		cont = 0;
	}

	if (frameH == TARROCANT -1){
		frameH = 0;
	}
	apply_surface( offSetx, offSety, tarros, screen, &tarroclip[ frameH ] );
	status++;
}
