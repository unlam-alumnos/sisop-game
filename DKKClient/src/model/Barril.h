/*
 * Barril.h
 *
 *  Created on: Nov 3, 2014
 *      Author: cristianmiranda
 */

#ifndef MODEL_BARRIL_H_
#define MODEL_BARRIL_H_

#include "../../../DKK/src/utils/includes/General.h"
#include "Actor.h"
#include "../common/Constants.h"

class Barril : public Actor{
	public:
		Barril(): Actor(SCREEN_WIDTH / 2 + DKW / 2, FLOOR -BARRILH - 306){
			width = BARRILW;
			height = BARRILH;
			status = BARRIL_UNBORN;
		}
		int directionMultiPlier; // Right

		void move(int);

		void show(int);

		void born();

		void die();

		void init(int index, int direction);

	private:


};

#endif /* MODEL_BARRIL_H_ */
