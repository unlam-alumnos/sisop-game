/*
 * Kong.h
 *
 *  Created on: Nov 4, 2014
 *      Author: cristianmiranda
 */

#ifndef MODEL_KONG_H_
#define MODEL_KONG_H_

#include "Actor.h"
#include "../../../DKK/src/utils/includes/General.h"
#include "../core/Core.h"
#include "../common/Constants.h"
#include "../common/Objects.h"
#include "../common/Variables.h"
#include "../model/Barril.h"

class Kong: public Actor {
	public:
		Kong(): Actor(SCREEN_WIDTH / 2 - DKW / 2, FLOOR - DKH - 306){
			width = DKW;
			height = DKH;
			status = KONG_FRONT_1;
		}

		void move();

		void show();

		void throwLeft();

		void throwRight();

};

#endif /* MODEL_KONG_H_ */
