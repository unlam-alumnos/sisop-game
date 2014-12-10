/*
 * Actor.cpp
 *
 *  Created on: Nov 3, 2014
 *      Author: cristianmiranda
 */

#include "Actor.h"

Actor::Actor(int x, int y)
{
    //Initialize movement variables
    offSetx = x;
    offSety = y;
    velocityX = 0;
    velocityY = 0;
    width = 0;
    height = 0;

    //Initialize animation variables
    frameH = 0;
    frameV = 0;
    status = 0;
    cont = 0;

    inALadder = NO_LADDER;
}

void Actor::move()
{
	//Move
	offSetx += velocityX;

	//Keep the stick figure in bounds
	if( offSetx < 0 ) {
		offSetx = 0;
	} else if ( offSetx > SCREEN_WIDTH - width +1) {
		offSetx = SCREEN_WIDTH - width +1;
	}


	int leftB, rightB, topB, bottomB;
	leftB = offSetx;
	rightB = offSetx + width;
	topB = offSety;
	bottomB = offSety + height;

	if((( ( leftB < SCREEN_WIDTH / 2 + TARROW / 2 ) &&
		//( topB >= FLOOR-PLAYERH && leftB > SCREEN_WIDTH / 2 - TARROW / 2) )||
		( (bottomB <= SCREEN_HEIGHT && bottomB > FLOOR - TARROH )  && leftB > SCREEN_WIDTH / 2 - TARROW / 2) )||
		( rightB >= SCREEN_WIDTH / 2 - TARROW / 2 &&
		//( topB >= FLOOR-PLAYERH && rightB < SCREEN_WIDTH / 2 + TARROW / 2) )))
		( (bottomB <= SCREEN_HEIGHT && bottomB > FLOOR - TARROH )&& rightB < SCREEN_WIDTH / 2 + TARROW / 2) )))
		{
		//offSetx = SCREEN_WIDTH / 2 + TARROW / 2;
		offSetx -= velocityX;
	}

}

int Actor::checkLadders(){
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftB = offSetx;
	rightB = offSetx + width;
	topB = offSety;
	bottomB = offSety + height;

	// Busco el bloque sobre el cual interactua Mario en ese momento
	int i = 0;
	while(i < LADDERS){
		SDL_Rect stair = ladders[i];
		leftA = stair.x;
		rightA = stair.x + stair.w;
		topA = stair.y;
		bottomA = stair.y + stair.h;

		if(leftB >= leftA && rightB <= rightA){
			if(abs(bottomB - bottomA) <= 3)
				return LADDER_BOTTOM;
			if(abs(bottomB - topA) <= 3)
				return LADDER_TOP;
			if((bottomA - bottomB) >= 3 && (bottomB - topA) >= 3)
				return LADDER_MIDDLE;
			// Está en la misma linea pero no a la altura, sigo buscando
		}
		i++;
	}
	return NO_LADDER;
}


int Actor::checkWalls(){
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftB = offSetx;
	rightB = offSetx + width;
	topB = offSety;
	bottomB = offSety + height;

	// Busco el bloque sobre el cual interactua Mario en ese momento
	int i = 0;
	int distance = SCREEN_HEIGHT;
	int iWall = 0;
	while(i < BLOCKS){
		SDL_Rect wall = walls[i];
		leftA = wall.x;
		rightA = wall.x + wall.w;
		topA = wall.y;
		bottomA = wall.y + wall.h;

		if((leftB >= leftA && leftB <= rightA) || (rightB >= leftA && rightB <= rightA)){
			int wallDistance = abs(bottomB - topA);
			if(wallDistance < distance){
				iWall = i;
				distance = wallDistance;
			}
		}
		i++;
	}

	return iWall;
}
