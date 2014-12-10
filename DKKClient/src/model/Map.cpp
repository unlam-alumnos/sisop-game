/*
 * Map.cpp
 *
 *  Created on: Nov 1, 2014
 *      Author: cristianmiranda
 */

#include "Map.h"

SDL_Rect *marioObj;
SDL_Rect *luigiObj;
SDL_Rect *paulineObj;
SDL_Rect *walls;
SDL_Rect *ladders;
SDL_Rect *barrels;
SDL_Rect *flames;

Map::Map() {
	init();
}

void Map::show()
{

    apply_surface( 10, 10, background, screen, NULL);
}

void Map::init() {

	marioObj = (SDL_Rect*) malloc(sizeof(SDL_Rect));
	luigiObj = (SDL_Rect*) malloc(sizeof(SDL_Rect));
	paulineObj = (SDL_Rect*) malloc(sizeof(SDL_Rect));
	walls = (SDL_Rect*) malloc(sizeof(SDL_Rect) * BLOCKS);
	ladders = (SDL_Rect*) malloc(sizeof(SDL_Rect) * LADDERS);
	barrels = (SDL_Rect*) malloc(sizeof(SDL_Rect) * BARRILES);
	flames = (SDL_Rect*) malloc(sizeof(SDL_Rect) * FUEGOS);

	mapGroundFloor();
	mapFirstFloor();
	mapSecondFloor();
	mapThirdFloor();
	mapFourthFloor();
	mapFifthFloor();
	mapSixthFloor();

}

void Map::mapGroundFloor(){
	mapStairs(0, 0, FLOOR - 13, false, true, 7);
	mapBigBlock(7, walls[6].x + walls[6].w, walls[6].y + 2, 319);
	mapStairs(8, walls[7].x + walls[7].w, walls[7].y - 2, true, true, 7);
	mapLadder(0, 34, FLOOR - 55 , 45);
	mapLadder(1, 182, FLOOR - 10, 10);
	mapLadder(2, 422, FLOOR - 10, 10);
	mapLadder(3, SCREEN_WIDTH - LADDER_WIDTH - 34, FLOOR - 55, 45);
}

void Map::mapFirstFloor(){
	mapStairs(15, 22, FLOOR - 53, true, true, 23);
	mapBigBlock(27, walls[26].x + walls[26].w, walls[26].y - 2, 46);
	mapStairs(28, walls[27].x + walls[27].w, walls[27].y + 2, false, true, 23);
	mapLadder(4, SCREEN_WIDTH - LADDER_WIDTH - 160, FLOOR - 124, 60);
	mapLadder(5, SCREEN_WIDTH - LADDER_WIDTH - 251, FLOOR - 117, 46);
	mapLadder(6, 251, FLOOR - 117, 46);
	mapLadder(7, 160, FLOOR - 124, 60);
}

void Map::mapSecondFloor(){
	mapStairs(40, 0, FLOOR - 137, false, true, 13);
	mapStairs(53, 342, FLOOR - 115, true, true, 13);
	mapLadder(8, 34, FLOOR - 179, 45);
	mapLadder(9, 137, FLOOR - 186, 60);
	mapLadder(10, 205, FLOOR - 128, 10);
	mapLadder(11, SCREEN_WIDTH - LADDER_WIDTH - 205, FLOOR - 128, 10);
	mapLadder(12, SCREEN_WIDTH - LADDER_WIDTH - 137, FLOOR - 186, 60);
	mapLadder(13, SCREEN_WIDTH - LADDER_WIDTH - 34, FLOOR - 179, 45);
}

void Map::mapThirdFloor(){
	mapStairs(66, 22, FLOOR - 177, true, true, 23);
	mapBigBlock(78, walls[77].x + walls[77].w, walls[77].y - 2, 46);
	mapStairs(79, walls[78].x + walls[78].w, walls[78].y + 2, false, true, 23);
	mapLadder(14, SCREEN_WIDTH - LADDER_WIDTH - 57, FLOOR - 187, 10);
	mapLadder(15, SCREEN_WIDTH - LADDER_WIDTH - 194, FLOOR - 245, 54);
	mapLadder(16, SCREEN_WIDTH - LADDER_WIDTH - 251, FLOOR - 241, 46);
	mapLadder(17, 251, FLOOR - 241, 46);
	mapLadder(18, 194, FLOOR - 245, 54);
	mapLadder(19, 57, FLOOR - 187, 10);
}

void Map::mapFourthFloor(){
	mapStairs(91, 0, FLOOR - 261, false, true, 13);
	mapStairs(104, 342, FLOOR - 239, true, true, 13);
	mapLadder(20, SCREEN_WIDTH - LADDER_WIDTH - 34, FLOOR - 303, 46);
	mapLadder(21, SCREEN_WIDTH - LADDER_WIDTH - 171, FLOOR - 261, 15);
	mapLadder(22, 171, FLOOR - 261, 15);
	mapLadder(23, 34, FLOOR - 303, 46);
}

void Map::mapFifthFloor(){
	mapStairs(117, 22, FLOOR - 301, true, true, 4);
	mapBigBlock(121, walls[120].x + walls[120].w, walls[120].y - 2, 411);
	mapStairs(122, walls[121].x + walls[121].w, walls[121].y + 2, false, true, 4);
	mapLadder(24, SCREEN_WIDTH - LADDER_WIDTH - 190, FLOOR - 381, 74);
	mapLadder(25, 190, FLOOR - 381, 74);
}

void Map::mapSixthFloor(){
	mapBigBlock(126, 199, FLOOR - 381, 239);
}

void Map::mapStairs(int wallIndex, int xFrom, int yFrom, bool goingUp, bool goingRight, int steps){
	walls[wallIndex].x = xFrom;
	walls[wallIndex].y = yFrom;
	for(int i = wallIndex; i < wallIndex + steps; i++){
		walls[i].w = 23;
		walls[i].h = 10;
	}
	for(int i = wallIndex + 1; i < wallIndex + steps; i++){
		walls[i].x = goingRight == true ? walls[i-1].x + walls[i-1].w : walls[i-1].x - walls[i-1].w;
		walls[i].y = goingUp == true ? walls[i-1].y - 2 : walls[i-1].y + 2;
	}
}

void Map::mapBigBlock(int wallIndex, int xFrom, int yFrom, int width){
	walls[wallIndex].x = xFrom;
	walls[wallIndex].y = yFrom;
	walls[wallIndex].w = width;
	walls[wallIndex].h = 10;
}

void Map::mapLadder(int ladderIndex, int xFrom, int yFrom, int height){
	ladders[ladderIndex].x = xFrom;
	ladders[ladderIndex].y = yFrom;
	ladders[ladderIndex].h = height;
	ladders[ladderIndex].w = LADDER_WIDTH;
}
