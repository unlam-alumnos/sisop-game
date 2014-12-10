/*
 * Character.h
 *
 *  Created on: Nov 5, 2014
 *      Author: cristianmiranda
 */

#ifndef MODEL_CHARACTER_CHARACTER_H_
#define MODEL_CHARACTER_CHARACTER_H_

#include "../Actor.h"

#include "../../../../DKK/src/utils/includes/Connection.h"
#include "../../../../DKK/src/common/SharedConstants.h"

class Character : public Actor {
	public:

		SDL_Surface *pj;
		SDL_Rect *rightClip;
		SDL_Rect *leftClip;

		// Uso un buffer para impedir que salte varias veces en el aire
		int jumpBuffer;

		// Socket utilizado para la conexión con el servidor de partida
		int socket;

		int paulines;

		bool isClient;

		bool isMario;

		char name[45];

		Character(int x, int y) : Actor(x, y){}

		void move();

		void show();

		//Maneja la tecla presionada y administra la velocidad
		void handle_input();

		void die();

		void win();

		void reborn();

	private:

		void notifyDeath();

		void notifyRescue();

		char* receiveEvents();

		bool checkBarrels();

		bool checkFlames();

		bool checkPauline();

};

#endif /* MODEL_CHARACTER_CHARACTER_H_ */
