/*
 * DKKMonitor.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: cristianmiranda
 */

#include "DKKMonitor.h"

#include "../../DKK/src/utils/log/LogUtils.c"

struct sMatchPid *serverMatchesPids;
int *isServerPresent;

void* checkServer(void*);
void* checkClient(void*);
void freeResourcesServer();
void freeResourcesClient();

int main(int argc, char* args[]) {

	logInFile(LOG_FILE_NAME, "Iniciando monitor");

	int pidFd = shm_open(SM_MATCHES_PIDS_NAME, O_CREAT | O_RDWR, 0644);
	if (pidFd == -1)
		logInFile(ERROR_FILE_NAME, "ERROR shm_open() para /smMatchesPids");
	
	if (ftruncate(pidFd, sizeof(struct sMatchPid) * 2) == -1)
		logInFile(ERROR_FILE_NAME, "ERROR ftruncate() para /smMatchesPids");
	
	serverMatchesPids = (struct sMatchPid *) malloc(sizeof(struct sMatchPid) * 2); // TODO Definir cantidad de partidas maxima

	serverMatchesPids = (struct sMatchPid *) mmap(NULL, sizeof(struct sMatchPid) * 2, PROT_READ | PROT_WRITE, MAP_SHARED, pidFd, 0);
	if (serverMatchesPids == MAP_FAILED)
		logInFile(ERROR_FILE_NAME,"ERROR mmap() para /smMatchesPids");

	int fd = shm_open(SM_IS_SERVER_PRESENT, O_CREAT | O_RDWR, 0644);
    if (fd == -1)
        logInFile(ERROR_FILE_NAME,"ERROR shm_open() para SM_IS_SERVER_PRESENT en el Monitor main()");

    if (ftruncate(fd, sizeof(sem_t)) == -1)
        logInFile(ERROR_FILE_NAME,"ERROR ftruncate() para SM_IS_SERVER_PRESENT en en el Monitor main()");

    isServerPresent = (int *)malloc(sizeof(int));
    
    isServerPresent = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (isServerPresent == MAP_FAILED)
        logInFile(ERROR_FILE_NAME,"ERROR mmap() para SM_IS_SERVER_PRESENT en en el Monitor main()");


	pthread_t threadCheckServer;
    if(*isServerPresent == 1){
	    if( pthread_create( &threadCheckServer , NULL ,  checkServer , (void*) 0 ) < 0)
	    	logInFile(ERROR_FILE_NAME, "No se pudo crear el thread que ejecuta checkServer()");
    }

    pthread_t threadCheckClient;
	if( pthread_create( &threadCheckClient , NULL ,  checkClient , (void*) 0 ) < 0)
		logInFile(ERROR_FILE_NAME,"Error al crear thread que ejecuta checkClient()");

	if(*isServerPresent == 1){
		if (pthread_join(threadCheckServer, NULL))
			logInFile(ERROR_FILE_NAME, "No se pudo hacer join del thread que ejecuta threadCheckServer");
	}
	
    if (pthread_join(threadCheckClient, NULL)) {
    	logInFile(ERROR_FILE_NAME, "Error al hacer joing del thread que ejecuta threadCheckClient");
	}

}	

void *checkServer(void *p){

	logInFile(LOG_FILE_NAME,"Inicio checkServer()");

    int port = 9970;
    char IPServer[] = "127.0.0.1";
    int sock;
    struct sockaddr_in server;
    char server_reply[100];
    sleep(10);
    while(1){

        // Crea el socket
        sock = socket(AF_INET , SOCK_STREAM , 0);

        if (sock == -1){
        	logInFile(ERROR_FILE_NAME,"[checkServer()] - No se pudo crear SOCKET que escribe al server en checkServer()");
        } else {
        
			logInFile(LOOP_FILE_NAME, "[checkServer()] - Se creo el SOCKET para chequear el Proceso Local");

			struct timeval tv;
			tv.tv_sec = 10; // Timeout
			tv.tv_usec = 0; // Not initializing this can cause strange errors
			setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

			// Preparo la estructura sockaddr_in
			server.sin_addr.s_addr = inet_addr(IPServer);
			server.sin_family = AF_INET;
			server.sin_port = htons( port );

			// Intenta conectarse al servidor
			if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
				logInFile(ERROR_FILE_NAME, "[checkServer()] - No se pudo conectar al Proceso Local. Se procederá a liberar recursos.");
				freeResourcesServer();
				break;
			} else {
				if(send(sock , "a" , sizeof(char) * 2, 0) < 0){
					logInFile(ERROR_FILE_NAME,"[checkServer()] - No se pudo enviar check de hombre muerto al Proceso Local.");
					freeResourcesServer();
					break;
				} else {
					char checkingChar[2];
					int retry = 0;
					do {
						if(recv(sock , checkingChar , sizeof(char) * 2, 0) < 0){
							logInFile(ERROR_FILE_NAME,"[checkServer()] - No se pudo recibir check de hombre muerto del Proceso Local.");
							logInfo(ERROR_FILE_NAME,"[checkServer()] - Reintentando recv() por %dº vez.", retry+1);
							send(sock , "a" , sizeof(char) * 2, 0);
						} else {
							logInFile(LOOP_FILE_NAME, "[checkServer()] - Se conecto con el Proceso Local");
							break;
						}
						retry++;
					} while(retry < 3);
					if (retry == 3){
						logInFile(LOG_FILE_NAME,"[checkServer()] - Se acabaron los intentos de recv(). Liberando recursos.");
						freeResourcesServer();
						break;
					}
				}
			}
			close(sock);
			sleep(5);
        }
    }
}

void *checkClient(void *p){

	logInFile(LOG_FILE_NAME,"Inicio checkClient()");

    int port = 9960;
    char IPServer[] = "127.0.0.1";
    int sock;
    struct sockaddr_in server;
    char server_reply[100];
    sleep(10);
    while(1){

        // Crea el socket
        sock = socket(AF_INET , SOCK_STREAM , 0);

        if (sock == -1){
        	logInFile(ERROR_FILE_NAME,"[checkClient()] - No se pudo crear SOCKET que escribe al server en checkClient()");
        } else {
        
			logInFile(LOOP_FILE_NAME, "[checkClient()] - Se creo el SOCKET para chequear el Proceso Local");

			struct timeval tv;
			tv.tv_sec = 20; // Timeout
			tv.tv_usec = 0; // Not initializing this can cause strange errors
			setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

			// Preparo la estructura sockaddr_in
			server.sin_addr.s_addr = inet_addr(IPServer);
			server.sin_family = AF_INET;
			server.sin_port = htons( port );

			// Intenta conectarse al servidor
			int connectRetry=0;
			do{
				if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
					logInFile(ERROR_FILE_NAME, "[checkClient()] - No se pudo conectar al Proceso Local.");
					logInfo(ERROR_FILE_NAME,"[checkClient()] - Reintentando connect() por %dº vez.", connectRetry+1);
					connectRetry++;
				}
			}while(connectRetry < 3);

			if (connectRetry == 3){
				logInFile(ERROR_FILE_NAME, "[checkClient()] - No se pudo conectar al Proceso Local. Se procederá a liberar recursos.");
				freeResourcesClient();
				break;
			} else {
				if(send(sock , "a" , sizeof(char) * 2, 0) < 0){
					logInFile(ERROR_FILE_NAME,"[checkClient()] - No se pudo enviar check de hombre muerto al Proceso Local.");
					freeResourcesClient();
					break;
				} else {
					char checkingChar[2];
					int retry = 0;
					do {
						if(recv(sock , checkingChar , sizeof(char) * 2, 0) < 0){
							logInFile(ERROR_FILE_NAME,"[checkClient()] - No se pudo recibir check de hombre muerto del Proceso Local.");
							logInfo(ERROR_FILE_NAME,"[checkClient()] - Reintentando recv() por %dº vez.", retry+1);
							send(sock , "a" , sizeof(char) * 2, 0);
						} else {
							logInFile(LOOP_FILE_NAME, "[checkClient()] - Se conecto con el Proceso Local");
							break;
						}
						retry++;
					} while(retry < 3);
					if (retry == 3){
						logInFile(LOG_FILE_NAME,"[checkClient()] - Se acabaron los intentos de recv(). Liberando recursos.");
						freeResourcesClient();
						break;
					}
				}
			}
			close(sock);
			sleep(5);
        }
    }
}

void freeResourcesServer(){

	logInFile(LOG_FILE_NAME,"[freeResourcesServer()] - Comenzando a liberar los recursos del server");

	int cantMatches = (int) sizeof(serverMatchesPids) / (int) sizeof(int);
	logInfo(LOG_FILE_NAME,"[freeResourcesServer()] - Cantidad de partidas a terminar: %d", cantMatches);

	for(int i = 0; i < cantMatches; i++){
		logInfo(LOG_FILE_NAME,"[freeResourcesServer()] - Eliminando proceso de servidor de partida. PID: %d", serverMatchesPids[i].pid);
		kill(serverMatchesPids[i].pid, SIGKILL);
	}

	logInfo(LOG_FILE_NAME,"[freeResourcesServer()] - Liberando semáforo: %s", SEM_SYNC_MATCH);
	sem_unlink(SEM_SYNC_MATCH);
	
	logInfo(LOG_FILE_NAME,"[freeResourcesServer()] - Liberando semáforo: %s", SEM_MONITOR_STARTED);
	sem_unlink(SEM_MONITOR_STARTED);


	logInfo(LOG_FILE_NAME,"[freeResourcesServer()] - Liberando memoria compartida: %s",SM_MONITOR_STARTED);
	shm_unlink(SM_MONITOR_STARTED); 
	
	logInfo(LOG_FILE_NAME,"[freeResourcesServer()] - Liberando memoria compartida: %s",SM_MATCHES_NAME);
	shm_unlink(SM_MATCHES_NAME);

	logInfo(LOG_FILE_NAME,"[freeResourcesServer()] - Liberando memoria compartida: %s",SM_MATCHES_PIDS_NAME);
	shm_unlink(SM_MATCHES_PIDS_NAME);

}


void freeResourcesClient(){

	logInFile(LOG_FILE_NAME,"[freeResourcesClient()] - Comenzando a liberar los recursos del client");

}
