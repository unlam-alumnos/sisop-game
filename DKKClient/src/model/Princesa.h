/*
 * Princesa.h
 *
 *  Created on: 6/11/2014
 *      Author: her
 */

#ifndef MODEL_PRINCESA_H_
#define MODEL_PRINCESA_H_

#include "Actor.h"
#include "../../../DKK/src/utils/includes/General.h"
#include "../core/Core.h"
#include "../common/Constants.h"
#include "../common/Objects.h"
#include "../common/Variables.h"

class Princesa: public Actor {
	public:

		int statusPrev;

		Princesa(): Actor(235, FLOOR - 404){
			width = PRINCESAW;
			height = PRINCESAH;
			status = PRINCESA_RIGHT;
			statusPrev = PRINCESA_RIGHT;
			frameH = 0;
			paulineObj->w = width;
			paulineObj->h = height;
		}

		void move();

		void show();


};

#endif /* MODEL_PRINCESA_H_ */
