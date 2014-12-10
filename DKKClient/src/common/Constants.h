/*
 * Constants.h
 *
 *  Created on: Oct 31, 2014
 *      Author: cristianmiranda
 */

#ifndef COMMON_CONSTANTS_H_
#define COMMON_CONSTANTS_H_

#include "../../../DKK/src/utils/includes/General.h"

#define LOG_FILE_NAME "DKKClient.log"
#define LOOP_FILE_NAME "DKKClientLoop.log"
#define ERROR_FILE_NAME "DKKClientError.log"

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//Stairs
const int LADDERS = 26;
const int LADDER_WIDTH = 35;
const int NO_LADDER = 0;
const int LADDER_BOTTOM = 1;
const int LADDER_MIDDLE = 2;
const int LADDER_TOP = 3;

const int GRAVITY = 7;

//Floor
const int FLOOR = SCREEN_HEIGHT - 13;

const int GROUND_FLOOR_BLOCKS = 15;
const int FIRST_FLOOR_BLOCKS = 25;
const int SECOND_FLOOR_BLOCKS = 26;
const int THIRD_FLOOR_BLOCKS = 25;
const int FOURTH_FLOOR_BLOCKS = 26;
const int FIFTH_FLOOR_BLOCKS = 9;
const int SIXTH_FLOOR_BLOCKS = 1;

const int BLOCKS = GROUND_FLOOR_BLOCKS +
		FIRST_FLOOR_BLOCKS +
		SECOND_FLOOR_BLOCKS +
		THIRD_FLOOR_BLOCKS +
		FOURTH_FLOOR_BLOCKS +
		FIFTH_FLOOR_BLOCKS +
		SIXTH_FLOOR_BLOCKS;

//The frame rate
const int FRAMES_PER_SECOND = 20;

//Medidas de los sprite
const int DKH = 38;
const int DKW = 52;
const int DKCANT = 5;

const int PLAYERH = 20;
const int PLAYERW = 20;
const int PLAYERCANT = 18;

const int BARRILH = 13;
const int BARRILW = 13;
const int BARRILFW = 20;
const int BARRILSCANT = 4;
const int BARRILFCANT = 2;
const int BARRILES = 2;

const int BARRILESH = 40;
const int BARRILESW = 26;
const int BARRILESCANT = 1;

const int PRINCESAH = 26;
const int PRINCESAW = 18;
const int PRINCESACANT = 3;

const int FUEGOH = 18;
const int FUEGOW = 16;
const int FUEGOCANT = 4;
const int FUEGOS = 2;

const int TARROH = 38;
const int TARROW = 20;
const int TARROCANT = 5;
const int TARRO_STATUS = 2;

//The direction status of the stick figure
const int MARIO_RIGHT = 0;
const int MARIO_LEFT = 1;
const int MARIO_UPDOWN = 2;
const int MARIO_JUMP = 5;
const int MARIO_DEAD = 6;
const int MARIO_HAPPY = 7;
const int MARIO_REBORN = 10;

const int BARRIL_BORN = 98;
const int BARRIL_UNBORN = 99;

const int FUEGO_BORN = 98;
const int FUEGO_UNBORN = 99;

const int DIR_RIGHT = 1;
const int DIR_LEFT = -1;

const int BARRIL_SIDE_1 = 0;
const int BARRIL_SIDE_2 = 1;
const int BARRIL_SIDE_3 = 2;
const int BARRIL_SIDE_4 = 3;
const int BARRIL_FRONT_1 = 4;
const int BARRIL_FRONT_2 = 5;

const int KONG_FRONT_1 = 0;
const int KONG_LEFT = 1;
const int KONG_FRONT_2 = 2;
const int KONG_RIGHT = 3;
const int KONG_BARRELL= 4;

const int PRINCESA_LEFT = 0;
const int PRINCESA_RIGHT = 1;
const int PRINCESA_HAPPY = 2;
const int PRINCESA_LIMITR = (SCREEN_WIDTH / 2) + 50;
const int PRINCESA_LIMITL = (SCREEN_WIDTH / 2) - 50;

#endif /* COMMON_CONSTANTS_H_ */
