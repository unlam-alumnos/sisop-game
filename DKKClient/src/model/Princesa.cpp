/*
 * Princesa.cpp
 *
 *  Created on: 6/11/2014
 *      Author: her
 */

#include "Princesa.h"

void Princesa::move() {
	if(status != PRINCESA_HAPPY ){
		int rightB = offSetx + width;
		int leftB = offSetx;
		if(status == PRINCESA_RIGHT){
			if(rightB <= PRINCESA_LIMITR && leftB >= PRINCESA_LIMITL){
				velocityX = 2;
			} else {
				velocityX = 0;
				status = PRINCESA_LEFT;
				offSetx = PRINCESA_LIMITR - width;
			}
		} else {
			if(rightB <= PRINCESA_LIMITR && leftB >= PRINCESA_LIMITL){
				velocityX = -2;
			} else {
				velocityX = 0;
				status = PRINCESA_RIGHT;
				offSetx = PRINCESA_LIMITL;
			}
		}
		offSetx += velocityX;
		paulineObj->x = offSetx;
		paulineObj->y = offSety;
	}
}


void Princesa::show() {
	cont++;
	if( velocityX < 0 )
	{
		//Set the animation to left
		status = PRINCESA_LEFT;

		//Move to the next frame in the animation
		if(cont == 3){
			frameH++;
			cont = 0;
		}
	}
	//If Foo is moving right
	else if( velocityX > 0)
	{
		//Set the animation to right
		status = PRINCESA_RIGHT;

		//Move to the next frame in the animation
		if(cont == 3){
			frameH++;
			cont = 0;
		}
	}
	//If Foo standing
	else
	{
		//Restart the animation
		frameH = 0;
		cont = 0;
	}

	//Loop the animation
	if( frameH >= 3)
	{
		frameH = 0;
		cont = 0;
	}

	if(status != PRINCESA_HAPPY ){

		//Show the stick figure
		if( status == PRINCESA_RIGHT )
		{
			apply_surface( offSetx, offSety, pauline, screen, &princesaRclip[ frameH ] );
			statusPrev = PRINCESA_RIGHT;
		}
		else if( status == PRINCESA_LEFT )
		{
			apply_surface( offSetx, offSety, pauline, screen, &princesaLclip[ frameH ] );
			statusPrev = PRINCESA_LEFT;
		}
	}else{
		if(cont == 20){
			cont = 0;
			status = statusPrev;
		}else{
			if( statusPrev == PRINCESA_RIGHT )
			{
				apply_surface( offSetx, offSety, pauline, screen, &princesaRclip[ frameH ] );
				statusPrev = PRINCESA_RIGHT;
			}
			else if( statusPrev == PRINCESA_LEFT )
			{
				apply_surface( offSetx, offSety, pauline, screen, &princesaLclip[ frameH ] );
				statusPrev = PRINCESA_LEFT;
			}
		}
	}
}

