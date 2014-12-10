//============================================================================
// Name        : DKKMatchServer.cpp
// Author      : Cristian Miranda
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "../../DKK/src/utils/log/LogUtils.c"
#include "../../DKK/src/utils/string/StringUtils.c"

#include "DKKMatchServer.h"

void *communicate(void* id);
void routeMessages(int mySocket, int hisSocket, int matchId, char* playerId);

struct sInfoMatch *infoMatch;
int i;
int totalMatches;

int main(int argc, char const *argv[]){

	char auxChar[500];

	char *shareMemoryName = (char *)malloc(sizeof(char)*strlen(argv[1]));
	strcpy(shareMemoryName,argv[1]);
	
	i = atoi(argv[2]);
	totalMatches = atoi(argv[3]);
	
	infoMatch = (struct sInfoMatch *)malloc(sizeof(struct sInfoMatch) * totalMatches);

	int fd;

	// Create shared memory object and set its size */
	fd = shm_open(shareMemoryName, O_CREAT | O_RDWR, 0644);
	if (fd == -1)
		logInfo(ERROR_FILE_NAME,"[Match #%d] - ERROR shm_open()", i);


	if (ftruncate(fd, sizeof(struct sInfoMatch)) == -1)
		logInfo(ERROR_FILE_NAME,"[Match #%d] - ERROR ftruncate()", i);

	// Map shared memory object
	infoMatch = (struct sInfoMatch *) mmap(NULL, sizeof(struct sInfoMatch), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (infoMatch == MAP_FAILED)
		logInfo(ERROR_FILE_NAME,"[Match #%d] - ERROR mmap()", i);

	logInfo(LOG_FILE_NAME,"[Match #%d] - Ejecutado %s", i, shareMemoryName);
	logInfo(LOG_FILE_NAME,"[Match #%d] - sockPlayer1: %d", i, infoMatch[i].player1.socket);
	logInfo(LOG_FILE_NAME,"[Match #%d] - sockPlayer2: %d", i, infoMatch[i].player2.socket);
	logInfo(LOG_FILE_NAME,"[Match #%d] - P1 score: %d", i, infoMatch[i].player1.score);
	logInfo(LOG_FILE_NAME,"[Match #%d] - P2 score: %d", i, infoMatch[i].player2.score);

	pthread_t matchThread[2];
	for (int j = 0; j < 2; j++) {
		int *h = (int*)malloc(sizeof(int));
		*h = j;
		if( pthread_create( &matchThread[j] , NULL ,  communicate, (void*) h) < 0){
			logInfo(ERROR_FILE_NAME,"[Match #%d] - No se pudo crear thread que invoca a communicate()", i);
		}
	}
	for (int j = 0; j < 2; j++){
		if (pthread_join(matchThread[j], NULL)) {
			logInfo(ERROR_FILE_NAME,"[Match #%d] - Error joining thread que invoca a communicate()", i);
		}
	}

	return 0;

}

void *communicate(void* index) {

	int * h = (int *)index;
	int ix = *(h);

	char client_message[100];
	char indexChar[2];

	if (ix == 0){
		// DETERMINAR JUGADOR 1
		char *player1Name = (char*) malloc(sizeof(char) * 45);
		strcpy(indexChar,"0");
		send( infoMatch[i].player1.socket , indexChar , sizeof(char) * 2, 0); // Envia codigo 0 --> Jugador 1

		// Determina nombre del jugador
		if(recv(infoMatch[i].player1.socket , player1Name , sizeof(char) * 45 , 0) > 0) {
			logInfo(LOG_FILE_NAME,"[Match #%d] - Nombre obtenido del cliente: %s", i, player1Name);
			player1Name[45] = '\0';
			strcpy(infoMatch[i].player1.name, player1Name);
			if(send( infoMatch[i].player2.socket , player1Name , sizeof(char) * 45, 0) < 0){
				logInfo(LOG_FILE_NAME,"[Match #%d] - Fallo enviando nombre del jugador 1", i);
			}
		}
		free(player1Name);

		// Maneja los distintos mensajes
		routeMessages(infoMatch[i].player1.socket , infoMatch[i].player2.socket , i, indexChar);
	}else{
		// DETERMINAR JUGADOR 2
		char *player2Name = (char*) malloc(sizeof(char) * 45);
		strcpy(indexChar,"1");
		send( infoMatch[i].player2.socket , indexChar , sizeof(char) * 2, 0); // Envia codigo 1 --> Jugador 2

		// Determina nombre del jugador
		if(recv(infoMatch[i].player2.socket , player2Name , sizeof(char) * 45 , 0) > 0) {
			logInfo(LOG_FILE_NAME,"[Match #%d] - Nombre obtenido del cliente: %s", i, player2Name);
			player2Name[45] = '\0';
			strcpy(infoMatch[i].player2.name, player2Name);
			if(send( infoMatch[i].player1.socket , player2Name , sizeof(char) * 45, 0) < 0){
				logInfo(LOG_FILE_NAME,"[Match #%d] - Fallo enviando nombre del jugador 2", i);
			}
		}
		free(player2Name);

		// Maneja los distintos mensajes
		routeMessages(infoMatch[i].player2.socket , infoMatch[i].player1.socket , i, indexChar);
	}

}

void routeMessages(int mySocket, int hisSocket, int matchId, char* playerId) {
	// Recibe un mensaje del cliente
	char message[100];
	int read_size;
	while( (read_size = recv(mySocket , message , sizeof(char) * 100 , 0)) > 0 ) {
		// Si es un movimiento
		if (message != NULL) {
			if (str_contains(message, "=") && str_contains(message, "|") && str_contains(message, "_")){
				// Lo devuelvo al otro cliente...
				send(hisSocket , message , sizeof(char) * (strlen(message) + 1), 0);
			} else if (str_contains(message, "{rescue}")) { // Si rescató a Pauline...
				char** split = str_split(message, ':');
				if(str_equals(split[1], playerId)){
					logInfo(LOG_FILE_NAME,"[Match #%d] - %s rescató a pauline!!!", i, str_equals(split[1], "0")?infoMatch[i].player1.name:infoMatch[i].player2.name);
					send(hisSocket , message , sizeof(char) * (strlen(message) + 1), 0);
					(str_equals(split[1], "0"))?infoMatch[i].player1.rescues++:infoMatch[i].player2.rescues++;
				}
				free(split);
			} else if (str_contains(message, "{dead}")) { // Si murió...
				char** split = str_split(message, ':');
				if(str_equals(split[1], playerId)){
					logInfo(LOG_FILE_NAME,"[Match #%d] - %s ha muerto", i, str_equals(split[1], "0")?infoMatch[i].player1.name:infoMatch[i].player2.name);
					send(hisSocket , message , sizeof(char) * (strlen(message) + 1), 0);
					(str_equals(split[1], "0"))?infoMatch[i].player1.deaths++:infoMatch[i].player2.deaths++;
				}
				free(split);
			}
		}
	}
	if(read_size == 0){
		logInfo(LOG_FILE_NAME,"[Match #%d] - El cliente se desconecto", i);
	}else if(read_size == -1){
		logInfo(LOG_FILE_NAME,"[Match #%d] - Fallo la recepcion de mensajes del cliente", i);
	}
}
