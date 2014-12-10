/*
 * CommonStructs.h
 *
 *  Created on: Oct 28, 2014
 *      Author: cristianmiranda
 */

#ifndef UTILS_STRUCTS_COMMONSTRUCTS_H_
#define UTILS_STRUCTS_COMMONSTRUCTS_H_

typedef struct sPlayer {
    char name[45];
	int socket;
    int score;
    int rescues;
    int deaths;
}sPlayer;

typedef struct sInfoMatch {
	struct sPlayer player1;
	struct sPlayer player2;
}sInfoMatch;

typedef struct sMatchPid {
	pid_t pid;
}sMatchPid;

#endif /* UTILS_STRUCTS_COMMONSTRUCTS_H_ */
