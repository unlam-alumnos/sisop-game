/*
 * Tarro.h
 *
 *  Created on: 5/11/2014
 *      Author: her
 */

#ifndef MODEL_TARRO_H_
#define MODEL_TARRO_H_

#include "Actor.h"
#include "../../../DKK/src/utils/includes/General.h"
#include "../core/Core.h"
#include "../common/Constants.h"
#include "../common/Objects.h"
#include "../common/Variables.h"

class Tarro : public Actor {
	public:
		Tarro() : Actor(SCREEN_WIDTH / 2 - TARROW / 2, FLOOR - TARROH){
			width = TARROW;
			height = TARROH;
			status = 0;
			cont = 0;
		}

		void move();

		void show();


};

#endif /* MODEL_TARRO_H_ */
