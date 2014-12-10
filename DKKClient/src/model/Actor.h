/*
 * Actor.h
 *
 *  Created on: Nov 3, 2014
 *      Author: cristianmiranda
 */

#ifndef MODEL_ACTOR_H_
#define MODEL_ACTOR_H_

#include "../../../DKK/src/utils/includes/General.h"
#include "../core/Core.h"
#include "../common/Constants.h"
#include "../common/Objects.h"
#include "../common/Variables.h"
#include "../../../DKK/src/utils/log/LogUtils.h"

class Actor {

	public:

		Actor();

		Actor(int x, int y);

		//The offset
		int offSetx;
		int offSety;

		//Its rate of movement
		int velocityX;
		int velocityY;

		//Its current frame
		int frameH;
		int frameV;

		int width;
		int height;

		//The number of player
		int player;

		//Its animation status
		int status;

		int inALadder;

		int cont;

		int checkWalls();

		int checkLadders();

		//Mueve al actor
		void move();

		//Muestra al actor
		void show();

};

#endif /* MODEL_ACTOR_H_ */
