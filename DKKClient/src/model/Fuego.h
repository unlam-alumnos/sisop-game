/*
 * Fuego.h
 *
 *  Created on: 7/11/2014
 *      Author: her
 */

#ifndef MODEL_FUEGO_H_
#define MODEL_FUEGO_H_

#include "../../../DKK/src/utils/includes/General.h"
#include "Actor.h"
#include "../common/Constants.h"

class Fuego : public Actor{
	public:
		Fuego(): Actor( SCREEN_WIDTH / 2, FLOOR - FUEGOH){
			width = FUEGOW;
			height = FUEGOH;
			status = FUEGO_UNBORN;
			cont = 0;
		}
		int directionMultiPlier; // Right

		void move(int);

		void show(int);

		void born();

		void die();

		void init(int index, int direction);

	private:


};

#endif /* MODEL_FUEGO_H_ */
