/*
 * Luigi.h
 *
 *  Created on: Nov 5, 2014
 *      Author: cristianmiranda
 */

#ifndef MODEL_LUIGI_H_
#define MODEL_LUIGI_H_

#include "Character.h"
#include "../../../../DKK/src/utils/includes/General.h"
#include "../../core/Core.h"
#include "../../common/Constants.h"
#include "../../common/Objects.h"
#include "../../common/Variables.h"

#include "../../../../DKK/src/utils/log/LogUtils.h"
#include "../../../../DKK/src/utils/string/StringUtils.h"

class Luigi : public Character {

	public:

		Luigi() : Character(SCREEN_WIDTH / 2 + 50, FLOOR - PLAYERH) {
			rightClip = luigiRclip;
			leftClip = luigiLclip;
			width = PLAYERW;
			height = PLAYERH;
			status = MARIO_LEFT;
			jumpBuffer = 0;
			cont = 0;
			isMario = false;
			luigiObj->w = width;
			luigiObj->h = height;
		}

};

#endif /* MODEL_LUIGI_H_ */
