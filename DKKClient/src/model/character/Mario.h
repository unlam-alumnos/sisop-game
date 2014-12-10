/*
 * Mario.h
 *
 *  Created on: Oct 31, 2014
 *      Author: cristianmiranda
 */

#ifndef MODEL_MARIO_H_
#define MODEL_MARIO_H_

#include "Character.h"
#include "../../../../DKK/src/utils/includes/General.h"
#include "../../core/Core.h"
#include "../../common/Constants.h"
#include "../../common/Objects.h"
#include "../../common/Variables.h"

#include "../../../../DKK/src/utils/log/LogUtils.h"
#include "../../../../DKK/src/utils/string/StringUtils.h"

class Mario : public Character
{
	public:

		Mario() : Character(SCREEN_WIDTH / 2 -50, FLOOR - PLAYERH) {
			rightClip = marioRclip;
			leftClip = marioLclip;
			width = PLAYERW;
			height = PLAYERH;
			status = MARIO_RIGHT;
			jumpBuffer = 0;
			cont = 0;
			paulines = 0;
			isMario = true;
			marioObj->w = width;
			marioObj->h = height;
		}

};

#endif /* MODEL_MARIO_H_ */
