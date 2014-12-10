/*
 * Map.h
 *
 *  Created on: Nov 1, 2014
 *      Author: cristianmiranda
 */

#ifndef MODEL_MAP_H_
#define MODEL_MAP_H_

#include "../core/Core.h"
#include "../common/Constants.h"
#include "../common/Objects.h"
#include "../common/Variables.h"

class Map {
	public:
		Map();
		void show();
		void init();
	private:
		void mapGroundFloor();
		void mapFirstFloor();
		void mapSecondFloor();
		void mapThirdFloor();
		void mapFourthFloor();
		void mapFifthFloor();
		void mapSixthFloor();

		void mapStairs(int wallIndex, int xFrom, int yFrom, bool goingUp, bool goingRight, int steps);
		void mapBigBlock(int wallIndex, int xFrom, int yFrom, int width);

		void mapLadder(int ladderIndex, int xFrom, int yFrom, int height);
};

#endif /* MODEL_MAP_H_ */
