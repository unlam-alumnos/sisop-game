/*
 * Character.cpp
 *
 *  Created on: Nov 5, 2014
 *      Author: cristianmiranda
 */

#include "Character.h"

#include <exception>

#include "../../../../DKK/src/utils/log/LogUtils.c"
#include "../../../../DKK/src/utils/string/StringUtils.c"

int stopMessage = 0;

void Character::handle_input() {
    //If a key was pressed and mario is not dead
    if( event.type == SDL_KEYDOWN )
    {
        //Adjust the volocitiX
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: velocityY -= PLAYERH / 4; break;
            case SDLK_DOWN: velocityY += PLAYERH / 4; break;
        	case SDLK_RIGHT: velocityX += PLAYERW / 4; break;
			case SDLK_LEFT: velocityX -= PLAYERW / 4; break;
			case SDLK_SPACE: jumpBuffer == 0 ? (jumpBuffer = PLAYERH * 4) : jumpBuffer; break; // Le asigno el doble, desde el piso hasta el tope y desde el tope al piso
        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        //Adjust the volocitiX
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: velocityY += PLAYERH / 4; break;
            case SDLK_DOWN: velocityY -= PLAYERH / 4; break;
            case SDLK_LEFT: velocityX += PLAYERW / 4; break;
            case SDLK_RIGHT: velocityX -= PLAYERW / 4; break;
        }
    }
}

void Character::move() {
	if(status != MARIO_DEAD && status != MARIO_HAPPY){

		bool movedByRemote = false;

		if(checkPauline() == true){
			win();
		}

		if(checkBarrels() == false && checkFlames() == false){

			if (this->isClient == true) {
				if(((velocityX == 0 && velocityY == 0) && stopMessage == 1) || (velocityX != 0 || velocityY != 0)){
					char *movement = (char*)malloc(sizeof(char)*16);
					sprintf(movement, "%d=%03d|%03d_%02d|%02d", isMario?0:1, offSetx, offSety, velocityX, velocityY);
					movement[16] = '\0';
					send(socket , movement , sizeof(char) * 16 , 0);
					free(movement);
					if((velocityX == 0 && velocityY == 0) && stopMessage == 1){
						stopMessage = 0;
					} else if ((velocityX != 0 || velocityY != 0)){
						stopMessage = 1;
					}
				}
			} else {
				char *eventData = receiveEvents();
				if(eventData != NULL){
					// Only if it's a movement...
					if (str_contains(eventData,"=") && str_contains(eventData,"|") && str_contains(eventData,"_")) {
						movedByRemote = true;
						char *movement = (char*) malloc(sizeof(char) * 16);
						for(int i=0; i < 17; i++)
							movement[i] = '\0';
						strncpy(movement, eventData , 16);
						movement[16] = '\0';
						free(eventData);
						try {
							char** split = str_split(movement, '=');
							char** values = str_split(split[1], '_');
							char** coords = str_split(values[0], '|');
							char** speed = str_split(values[1], '|');
							int pj = atoi(split[0]);
							if ((pj == 0 && isMario) || (pj == 1 && !isMario)) {
								offSetx = atoi(coords[0]);
								offSety = atoi(coords[1]);
								velocityX = atoi(speed[0]);
								velocityY = atoi(speed[1]);
							}
							free(split);
							free(coords);
							free(speed);
						} catch(const std::exception& e) {
							// Won't do nothing, not event logging this
						}
						free(movement);

					} else if (str_contains(eventData, "{rescue}")) { // Si el otro jugador rescató a Pauline...
						// TODO: sumar punto al otro jugador
					} else if (str_contains(eventData, "{dead}")) { // Si el otro jugador murió...
						// TODO: restar vida al otro jugador ???? hay vidas?
					}
				}
			}

			if (!movedByRemote) {

				Actor::move();

				inALadder = checkLadders();

				// Check floor blocks

				int wall = checkWalls();
				// Si Mario está en el aire (3 pixeles arriba de un bloque se considera aire, menos no)
				// Lo muevo verticalmente hacia abajo de a 7 pixeles (gravedad)
				if((abs((walls[wall].y - height) - offSety)) > 3){
					if(inALadder == NO_LADDER){ // Para que no se caiga de la escalera cuando no se mueve verticalmente
						if(jumpBuffer <= PLAYERH * 2){ // Si está en el tope del salto o por debajo (incluido el jumpBuffer == 0 --> no hay salto)
							offSety += GRAVITY; // Aplico gravedad
							jumpBuffer != 0 ? jumpBuffer -= PLAYERH / 2 : 0; // Si hay salto, decremento el buffer
						}
					}
				} else {
					// Hago que mario se pare sobre el bloque
					if(jumpBuffer == 0) // Si no hay salto, muevo a mario al bloque que le corresponde en el eje y
						offSety = walls[wall].y - height;
				}

				if(inALadder != NO_LADDER){
					if(inALadder == LADDER_BOTTOM){
						if(velocityY < 0){
							offSety += velocityY;
						}
					} else if(inALadder == LADDER_TOP){
						if(velocityY > 0){
							offSety += velocityY;
						}
					} else {
						offSety += velocityY;
					}
				} else {
					if(jumpBuffer > 0){ // Si está saltando
						if(jumpBuffer > PLAYERH * 2){ // y todavía no llegó al tope en el aire...
							offSety -= GRAVITY; // Decremento gravedad para que salte
							jumpBuffer -= PLAYERH / 2; // Decremento el buffer de salto
						}
					}
				}

			}

		} else {
			die();
		}
	}
}

void Character::die() {
	status = MARIO_DEAD;
	frameH = 6;
	cont = 0;
}

void Character::win() {
	status = MARIO_HAPPY;
	paulines++;
	cont = 0;
}

void Character::reborn() {
	frameH = 0;
	cont = 0;
	offSety = FLOOR - PLAYERH;
	if (isMario) {
		status = MARIO_RIGHT;
		offSetx = SCREEN_WIDTH / 2 - 50;
	} else {
		status = MARIO_LEFT;
		offSetx = SCREEN_WIDTH / 2 + 50;
	}
}

bool Character::checkBarrels() {

	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftB = offSetx;
	rightB = offSetx + width;
	topB = offSety;
	bottomB = offSety + height;

	// Busco el barril sobre el cual interactua Mario en ese momento
	int i = 0;
	while(i < BARRILES){
		SDL_Rect barrel = barrels[i];
		leftA = barrel.x;
		rightA = barrel.x + barrel.w;
		topA = barrel.y;
		bottomA = barrel.y + barrel.h;
		if( (leftB <= rightA && leftB >= leftA &&  topB <= topA && bottomB >= topA) ||
			(rightB >= leftA && rightB <= rightA &&  topB <= topA && bottomB >= topA) ||
			(topB == bottomA && ( (rightA <= rightB && rightA >= leftB) || (leftA >= leftB && leftA <= rightB ))) ||
			(bottomB == topA && ( (rightA <= rightB && rightA >= leftB) || (leftA >= leftB && leftA <= rightB ))) )
		{
			return true;
		}
		i++;
	}

	return false;

}

bool Character::checkFlames() {

	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftB = offSetx;
	rightB = offSetx + width;
	topB = offSety;
	bottomB = offSety + height;

	// Busco el barril sobre el cual interactua Mario en ese momento
	int i = 0;
	while(i < FUEGOS){
		SDL_Rect flame = flames[i];
		leftA = flame.x;
		rightA = flame.x + flame.w;
		topA = flame.y;
		bottomA = flame.y + flame.h;
		if( (leftB <= rightA && leftB >= leftA &&  topB <= topA && bottomB >= topA) ||
			(rightB >= leftA && rightB <= rightA &&  topB <= topA && bottomB >= topA) ||
			(topB == bottomA && ( (rightA <= rightB && rightA >= leftB) || (leftA >= leftB && leftA <= rightB ))) ||
			(bottomB == topA && ( (rightA <= rightB && rightA >= leftB) || (leftA >= leftB && leftA <= rightB ))) )
		{
			return true;
		}
		i++;
	}

	return false;

}

bool Character::checkPauline() {

	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftB = offSetx;
	rightB = offSetx + width;
	topB = offSety;
	bottomB = offSety + height;

	// Busco el barril sobre el cual interactua Mario en ese momento

	leftA = paulineObj->x;
	rightA = paulineObj->x + paulineObj->w;
	topA = paulineObj->y;
	bottomA = paulineObj->y + paulineObj->h;
	if( (leftB <= rightA && leftB >= leftA &&   bottomB <= bottomA) ||
		(rightB >= leftA && rightB <= rightA &&   bottomB <= bottomA) )
	{
		return true;
	}

	return false;

}

void Character::show() {

    //Show the Mario
    //apply_surface( x, y, mario, screen );
	if(status != MARIO_DEAD && status != MARIO_HAPPY){

		//If Foo is moving left
		if( velocityX < 0 && velocityY == 0)
		{
			//Set the animation to left
			status = MARIO_LEFT;

			//Move to the next frame in the animation
			frameH++;
		}
		//If Foo is moving right
		else if( velocityX > 0 && velocityY == 0)
		{
			//Set the animation to right
			status = MARIO_RIGHT;

			//Move to the next frame in the animation
			frameH++;
		}
		//If Foo standing
		else
		{
			//Restart the animation
			frameH = 0;
		}

		//Loop the animation
		if( frameH >= 3)
		{
			frameH = 0;
		}


		//Show the stick figure
		if( status == MARIO_RIGHT )
		{
			apply_surface( offSetx, offSety, pj, screen, &rightClip[ frameH ] );
		}
		else if( status == MARIO_LEFT )
		{
			apply_surface( offSetx, offSety, pj, screen, &leftClip[ frameH ] );
		}

		if(checkLadders()) {
			//If Foo is moving left
			if( (velocityY > 0 && inALadder == LADDER_TOP) ||
				(velocityY < 0 && inALadder == LADDER_BOTTOM) ||
				(velocityY != 0 && inALadder == LADDER_MIDDLE))
			{
				//Set the animation to left
				status = MARIO_UPDOWN;

				//Move to the next frameH in the animation
				frameV++;
			}
			//If Foo standing
			else
			{
				//Restart the animation
				frameV = 3;
			}

			//Loop the animation
			if( frameV >= 5 )
			{
				frameV = 3;
			}

			//Show the stick figure
			if( status == MARIO_UPDOWN)
			{
				apply_surface( offSetx, offSety, pj, screen, &leftClip[ frameV ] );
			}
		}

		if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
			//No estoy en la escalera, estoy saltando
			if (velocityY < 0){
				//Muestro la imagen al salto correspondiente
				if( status == MARIO_RIGHT ) {
					apply_surface( offSetx, offSety, pj, screen, &rightClip[ MARIO_JUMP ] );
				} else if( status == MARIO_LEFT ) {
					apply_surface( offSetx, offSety, pj, screen, &leftClip[ MARIO_JUMP ] );
				}
			}
		}
	}else{
		if(status != MARIO_HAPPY){
			apply_surface( offSetx, offSety, pj, screen, &leftClip[ MARIO_DEAD ] );
			//Con esto doy un tiempo para mantener congelada la imagen del mario muerto
			cont++;
			if (cont == 15){
				notifyDeath();
				// TODO: restar vida a jugador local? hay vidas???
				reborn();
			}
		}else{
			apply_surface( offSetx, offSety, pj, screen, &leftClip[ MARIO_DEAD ] );
			cont++;
			if(cont == 20){
				notifyRescue();
				// TODO: sumar punto a jugador local
				reborn();
			}
		}
	}
}

void Character::notifyDeath() {
	char dead[] = "{dead}:";
	char *data = (char*) malloc(sizeof(char) * (strlen(dead) + 1));
	strcpy(data, dead);
	strcat(data, isMario?"0":"1");
	send(socket, data, sizeof(char) * (strlen(data) + 1) , 0);
}

void Character::notifyRescue() {
	char rescue[] = "{rescue}:";
	char *data = (char*) malloc(sizeof(char) * (strlen(rescue) + 1));
	strcpy(data, rescue);
	strcat(data, isMario?"0":"1");
	send(socket, data, sizeof(char) * (strlen(data) + 1) , 0);
}

char* Character::receiveEvents(){
	char *data = (char*)malloc(sizeof(char)*16);
	if(recv(socket, data, sizeof(char) * 16, 0) <= 0 ){
		free(data);
		data = NULL;
	}
	return data;
}
