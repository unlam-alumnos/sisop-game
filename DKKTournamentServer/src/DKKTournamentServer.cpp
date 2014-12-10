//============================================================================
// Name        : DKKTournamentServer.cpp
// Author      : Cristian Miranda
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "DKKTournamentServer.h"

#include "../../DKK/src/utils/file/FileUtils.c"
#include "../../DKK/src/utils/log/LogUtils.c"
#include "../../DKK/src/utils/string/StringUtils.c"
#include "../../DKK/src/utils/structs/CommonStructs.h"

#include "../../DKK/src/common/SharedConstants.h"

void loadProperties();
void initServer();

void *startMatches(void *index);
void *listenToMonitor(void *parameter);
void *refreshEvents(void *cantPartidas);

void *generateMatch(void * matchID);
void launchMatch(char *matchServerBinPath, char *args, char *index,char *cantTotalMatches);
void launchMonitor();
void *startMonitor(void *);

void manageSignal();
void signalHandler(int);

void *generateScreen(void * matchID);
SDL_Surface *load_image( std::string filename );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip );
bool init();
bool load_files();
void clean_up(int cantPartidas);
void generateText(int cantClients, SDL_Surface** partida);

//The surfaces
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface **ptrTitulosPartidas;

TTF_Font *font = NULL;
SDL_Color textColor = {220, 0, 0};

//The event structure that will be used
SDL_Event event;

//Screen attributes
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

int lastClient;
int totalMatches;
char cantTotalMatches[4];
struct sInfoMatch *infoMatch;
struct sMatchPid *serverMatchesPids;
int *isServerPresent;

int arrayClientSockets[20] = {0};
int indexClient1 = 0;
int indexClient2 = 1;

sem_t *semSyncGenerateMatch;
sem_t *semMonitorStarted;

//Pixeles del interlineado
const int interline = 20;

// Properties
char *matchServerPath;
char *monitorPath;
char *monitorPort;

int main() {

	// Carga el archivo de configuración
	loadProperties();

	// Lanza el proceso monitor
	pthread_t startMonitorThread;
    if( pthread_create( &startMonitorThread , NULL ,  startMonitor , (void*) 0 ) < 0)
        logInFile(ERROR_FILE_NAME,"Error al crear thread que ejecuta startMonitor()");

	// Inicia el server
	initServer();

    if (pthread_join(startMonitorThread, NULL)) {
        logInFile(ERROR_FILE_NAME, "Error al hacer joing del thread que ejecuta startMonitor()");
    }

}

void loadProperties(){
	logInFile(LOG_FILE_NAME,"Leyendo archivo de propiedades");
	matchServerPath = getProperty(PROPERTIES_FILE_NAME, "match.server.path");
	monitorPath = getProperty(PROPERTIES_FILE_NAME, "monitor.path");
	monitorPort = getProperty(PROPERTIES_FILE_NAME, "monitor.port");
}

void initServer() {


    int fd = shm_open(SM_IS_SERVER_PRESENT, O_CREAT | O_RDWR, 0644);
    if (fd == -1)
        logInFile(ERROR_FILE_NAME,"ERROR shm_open() para SM_IS_SERVER_PRESENT en initServer()");

    if (ftruncate(fd, sizeof(sem_t)) == -1)
        logInFile(ERROR_FILE_NAME,"ERROR ftruncate() para SM_IS_SERVER_PRESENT en initServer()");

    isServerPresent = (int *)malloc(sizeof(int));
    
    isServerPresent = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (isServerPresent == MAP_FAILED)
        logInFile(ERROR_FILE_NAME,"ERROR mmap() para SM_IS_SERVER_PRESENT en initServer()");

    *isServerPresent = 1;

	pthread_t mainThreads[2];
	pthread_create( &mainThreads[0] , NULL ,  startMatches , (void*) NULL );
	pthread_create( &mainThreads[1] , NULL ,  listenToMonitor , (void*) NULL );
	for (int j = 0; j < 2; j++){
		if (pthread_join(mainThreads[j], NULL)) {
			logInFile(ERROR_FILE_NAME,"Error joining main thread");
		}
	}
}

void *startMatches(void *arg) {

	logInFile(LOG_FILE_NAME, "Starting DDKTournamentServer");

	char *hostValue = getProperty(PROPERTIES_FILE_NAME, "server.host");
	char *portValue = getProperty(PROPERTIES_FILE_NAME, "server.port");
	logInfo(LOG_FILE_NAME,"host:puerto --> %s:%s", hostValue, portValue);

	char *timeOutConnections =  getProperty(PROPERTIES_FILE_NAME, "connection.timeout");
	logInfo(LOG_FILE_NAME,"Timeout de inscripción: %s", timeOutConnections);

	int port =  atoi(portValue);
	int socket_desc;
	int client_sock;
	int c;
	struct sockaddr_in server;
	struct sockaddr_in client;
	int cantClients = 0;

	// Create socket --> int socket(int domain, int type, int protocol);
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1){
		logInFile(ERROR_FILE_NAME, "No se pudo crear socket para escuchar clientes");
	}

	// Convierto el socket en no bloqueate
	fcntl(socket_desc, F_SETFL, O_NONBLOCK);

	// Preparo la estructura sockaddr_in
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	// Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
		logInFile(ERROR_FILE_NAME, "No se pudo enlazar socket para escuchar clientes");
	}

	// Empieza a escuchar
	listen(socket_desc , 3);
	logInfo(LOG_FILE_NAME,"Escuchando clientes en el puerto: %d", port);

	c = sizeof(struct sockaddr_in);
	int auxSeconds = 0;
	clock_t start = clock();
	while( auxSeconds <= atoi(timeOutConnections) ) {
		if ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) > 0){
			arrayClientSockets[cantClients] = client_sock;
			cantClients++;
			logInfo(LOG_FILE_NAME,"Cliente conectado. Socket #%d", client_sock);
		}

		clock_t end = clock();
		auxSeconds = (float)(end - start) / CLOCKS_PER_SEC;
	}

	if(cantClients >= 2){
        
        lastClient =  cantClients-1;

        infoMatch = (struct sInfoMatch *)malloc(sizeof(struct sInfoMatch) * cantClients/2);

        totalMatches = cantClients-1;
        serverMatchesPids = (struct sMatchPid *) malloc(sizeof(struct sMatchPid) * totalMatches); 

        char aux[4];
        sprintf(aux,"%d",totalMatches);
        strcpy(cantTotalMatches,aux);

        /*
         * infoMatch shared memory
         */
        int fd;
        // Create shared memory object and set its size */
        fd = shm_open(SM_MATCHES_NAME, O_CREAT | O_RDWR, 0644);
        if (fd == -1)
            logInFile(ERROR_FILE_NAME,"[Matches] - ERROR shm_open()");

        if (ftruncate(fd, sizeof(struct sInfoMatch)) == -1)
            logInFile(ERROR_FILE_NAME,"[Matches] - ERROR ftruncate()");

        // Map shared memory object
        infoMatch = (struct sInfoMatch *)mmap(NULL, sizeof(struct sInfoMatch), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (infoMatch == MAP_FAILED)
            logInFile(ERROR_FILE_NAME,"[Matches] - ERROR mmap()");

        /*
		 * serverMatchesPids shared memory
		 */
        int pidFd;
		// Create shared memory object and set its size */
        pidFd = shm_open(SM_MATCHES_PIDS_NAME, O_CREAT | O_RDWR, 0644);
		if (pidFd == -1)
			logInFile(ERROR_FILE_NAME,"[Matches PIDS] - ERROR shm_open()");

		if (ftruncate(pidFd, sizeof(sMatchPid) * (cantClients / 2)) == -1)
			logInFile(ERROR_FILE_NAME,"[Matches PIDS] - ERROR ftruncate()");

		// Map shared memory object
		serverMatchesPids = (struct sMatchPid *)mmap(NULL, sizeof(struct sMatchPid) * (cantClients / 2), PROT_READ | PROT_WRITE, MAP_SHARED, pidFd, 0);
		if (serverMatchesPids == MAP_FAILED)
			logInFile(ERROR_FILE_NAME,"[Matches PIDS] - ERROR mmap()");

		logInFile(LOG_FILE_NAME,"Memoria compartida mapeada");

		// Creo los semaforos
		semSyncGenerateMatch = sem_open (SEM_SYNC_MATCH, O_CREAT, 0644, 1);

		pthread_t matchThread[5]; // TODO alocar memoria dinamicamente por la cantidad de partidas totales que puede haber

		for (int i = 0; i < cantClients/2; i++){
			int *j = (int*)malloc(sizeof(int));
			*j = i;
			if( pthread_create( &matchThread[i] , NULL ,  generateMatch , (void*) j ) < 0){
				logInfo(ERROR_FILE_NAME,"No se pudo crear thread #%d que invoca a generateMatch()", i);
			}

		}

		// sleep(5); // TODO: Eliminar si no se usa

		//Initialize graphics
		SDL_Surface *titulosPartidas[cantClients/2];
		ptrTitulosPartidas = titulosPartidas;
		if( init() == false )
			logInFile(ERROR_FILE_NAME, "Fallo en el inicio de gráficos del servidor");

		pthread_t principalScreen;
		int *partidas = (int*)malloc(sizeof(int));
		*partidas = cantClients;
		if( pthread_create( &principalScreen , NULL ,  generateScreen , (void*) partidas ) < 0){
			logInFile(ERROR_FILE_NAME,"No se pudo crear thread que invoca a generateScreen()");
		}

		pthread_t eventsThread;
		if( pthread_create( &eventsThread , NULL ,  refreshEvents , (void*) partidas ) < 0){
			logInFile(ERROR_FILE_NAME,"No se pudo crear thread que invoca a refreshEvents()");
		}

		for (int j = 0; j < cantClients/2; j++){
			if (pthread_join(matchThread[j], NULL)) {
				logInfo(ERROR_FILE_NAME,"No se pudo hacer join del thread #%d que invoca a generateMatch()", j);
			}
		}

		if (pthread_join(principalScreen, NULL))
			logInFile(ERROR_FILE_NAME, "No se pudo hacer join del thread que invoca a generateScreen()");

		if (pthread_join(eventsThread, NULL))
			logInFile(ERROR_FILE_NAME, "No se pudo hacer join del thread que invoca a refreshEvents()");

		sem_unlink(SEM_SYNC_MATCH);
		sem_unlink(SEM_MONITOR_STARTED);
	}

}

void manageSignal(){

}

void signalHandler(int signal){

}

void *generateMatch(void * matchID){
    sem_wait(semSyncGenerateMatch);

    int * h = (int *)matchID;   
    int i = *(h);               
    logInfo(LOG_FILE_NAME,"[Match #%d] - Generando partida #%d", i, i);

    indexClient1 = i;
    indexClient2 = lastClient - i;

    logInfo(LOG_FILE_NAME,"[Match #%d - (%d vs. %d)] - Memoria compartida inicializada con éxito", i, arrayClientSockets[indexClient1], arrayClientSockets[indexClient2]);

    char index[5];
    sprintf(index,"%d",i);

    infoMatch[i].player1.socket = arrayClientSockets[indexClient1];
    infoMatch[i].player2.socket = arrayClientSockets[indexClient2];
    infoMatch[i].player1.score = 0;
    infoMatch[i].player2.score = 0;

    logInfo(LOG_FILE_NAME,"[Match #%d - (%d vs. %d)] - Memoria compartida inicializada con éxito", i, arrayClientSockets[indexClient1], arrayClientSockets[indexClient2]);

    logInfo(LOG_FILE_NAME,"[Match #%d] - Ejecutando partida: Cliente[%d] vs. Cliente[%d]", i, arrayClientSockets[indexClient1], arrayClientSockets[indexClient2]);

    launchMatch(matchServerPath, SM_MATCHES_NAME, index,cantTotalMatches);

}

void launchMatch(char *matchServerBinPath, char *args, char *index, char *cantTotalMatches){
    pid_t pid = fork();
    if (pid == -1){
    	logInFile(ERROR_FILE_NAME,"Error en fork() al ejecutar binario de servidor de partida");
    }else if (pid){
    	logInfo(LOG_FILE_NAME,"[Match #%s] - PID: %d", index, pid);
    	serverMatchesPids[atoi(index)].pid = pid;
        sem_post(semSyncGenerateMatch);
        wait(NULL);
    }else{
        execl(matchServerBinPath,matchServerBinPath,args,index,cantTotalMatches,NULL);
    }

}

void *startMonitor(void *params){

    int semFd = shm_open(SM_MONITOR_STARTED, O_CREAT | O_RDWR, 0644);
    if (semFd == -1)
        logInFile(ERROR_FILE_NAME,"ERROR shm_open() para SM_MONITOR_STARTED en startMonitor()");

    if (ftruncate(semFd, sizeof(sem_t)) == -1)
    	logInFile(ERROR_FILE_NAME,"ERROR ftruncate() para SM_MONITOR_STARTED en startMonitor()");

    semMonitorStarted = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, semFd, 0);
    if (semMonitorStarted == MAP_FAILED)
    	logInFile(ERROR_FILE_NAME,"ERROR mmap() para SEM_MONITOR_STARTED en startMonitor()");

    semMonitorStarted = sem_open (SEM_MONITOR_STARTED, O_CREAT, 0644, 1);

    launchMonitor();
}

void launchMonitor(){

	logInfo(LOG_FILE_NAME, "ANTES Iniciando Monitor en %s", monitorPath);

    sem_wait(semMonitorStarted);

    pid_t pid = fork();
    if (pid == -1){
    	logInFile(ERROR_FILE_NAME,"[Monitor] - Error en fork() al ejecutar binario del monitor");
    }else if(!pid){
    	logInfo(LOG_FILE_NAME,"[Monitor] - Ejecutando monitor en %s", monitorPath);
    	if(execv(monitorPath, NULL) < 0){
    		logInFile(ERROR_FILE_NAME,"[Monitor] - Fallo al ejecutar binario del monitor");
        }
    }
}

void *listenToMonitor(void *args) {

	logInFile(LOG_FILE_NAME, "Listening to DDKMonitor");

	char *portValue = getProperty(PROPERTIES_FILE_NAME, "monitor.port");
	char *hostValue = "localhost";
	logInfo(LOG_FILE_NAME,"[Monitor] - host:puerto --> %s:%s", hostValue, portValue);

	int port =  atoi(portValue);
	int socket_desc;
	int client_sock;
	int c;
	struct sockaddr_in server;
	struct sockaddr_in client;

	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1){
		logInFile(ERROR_FILE_NAME,"[Monitor] - No se pudo crear el SOCKET para escuchar al monitor en listenToMonitor()");
	} else {

		struct timeval tv;
		tv.tv_sec = 20; // Timeout
		tv.tv_usec = 0; // Not initializing this can cause strange errors
		setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

		// Preparo la estructura sockaddr_in
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(port);

		// Bind
		if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
			logInFile(ERROR_FILE_NAME,"[Monitor] - No se pudo enlazar el SOCKET para escuchar al monitor en listenToMonitor()");
		}

		// Empieza a escuchar
		listen(socket_desc , 3);
		logInfo(LOG_FILE_NAME,"Escuchando al monitor en el puerto: %d", port);

		c = sizeof(struct sockaddr_in);
		while(1) {
			if ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) > 0){
				char checkingChar[2];
				if(recv(client_sock , checkingChar , sizeof(char) * 2, 0) > 0){
					send(client_sock, "a", sizeof(char) * 2, 0);
					logInFile(LOOP_FILE_NAME, "Server escuchó al Monitor exitosamente.");
				} else {
					logInFile(LOG_FILE_NAME,"[Monitor] - No se pudo conectar al server Monitor. Relanzando servicio.");
                    sem_post(semMonitorStarted);    
					launchMonitor();
					break;
				}
			} else {
				logInFile(LOG_FILE_NAME,"[Monitor] - No se pudo conectar al server Monitor. Relanzando servicio.");
				sem_post(semMonitorStarted); 
                launchMonitor();
				break;
			}
		}
        
        logInfo(LOG_FILE_NAME,"[Monitor] - Cerrando socket --> %d.",socket_desc);
        close(socket_desc);
        
	}
}

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded

    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "DK Tournament", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the background image
    background = load_image( "gfx/images/backgroundServer800x600.png" );

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }


    //If everything loaded fine
    return true;
}

void clean_up(int cantPartidas)
{
    //Free the surfaces
    SDL_FreeSurface( background );

    for(int i = 0; i < cantPartidas; i++){
    	SDL_FreeSurface( (*ptrTitulosPartidas +i) );
    }


    //Close the font
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

void *generateScreen(void * cantClients){
	int * h = (int *)cantClients;
	int cantTotal = *(h);
	int cantPartidas = cantTotal / 2;

	//Inicio todos los msjs de partida en NULL
	for(int i = 0; i < cantPartidas; i++){
		*(ptrTitulosPartidas + i) = NULL;
	}


	//Quit flag
	bool quit = false;

	//Genero los textos de las partidas
	generateText(cantPartidas, ptrTitulosPartidas);

	//Load the files
	if( load_files() == false )
	{
		//return 1;
	}


	//Apply the background
	apply_surface( 0, 0, background, screen, NULL );

	//Muestro en pantalla las distintas partidas
	for(int i = 0; i < cantPartidas; i++){
		if( *(ptrTitulosPartidas + i) != NULL )
		{
			//Aplico los distintos titulos de partidas
			apply_surface( 10, 200 + (i * interline * 3), *(ptrTitulosPartidas + i), screen, NULL );

		}
	}


	//While the user hasn't quit
	while( quit == false )
	{
		//If there's an event to handle
		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT )
			{
				//Quit the program
				quit = true;
			}
		}

		//Update the screen
		if( SDL_Flip( screen ) == -1 )
		{
			//return 1;
		}
	}

	//Clean up
	clean_up(cantPartidas);

	//return 0;

}

void generateText(int cantPartidas, SDL_Surface **ptrTitulosPartidas){

	//Open the font
	//Diferenciar tamaño de linea para la cantidad de partidas
	font = TTF_OpenFont( "gfx/font/font.ttf", 20 );

	//String con todos los nombres
	char partidas[100];

	for(int i = 0; i < cantPartidas; i++){
		// Guardo los nombres de los participantes.
		sprintf(partidas, "[%s] vs. [%s]:", infoMatch[i].player1.name , infoMatch[i].player2.name);
		logInFile(LOG_FILE_NAME,partidas);
		*(ptrTitulosPartidas + i) = TTF_RenderText_Solid( font, partidas, textColor );
	}

}

void *refreshEvents(void * cantPartidas) {
	int *h = (int *)cantPartidas;
	while (1) {
		sleep(2);
		for (int i = 0; i < *h; i++) {
			// Si es una partida en curso o ya jugada
			if(!str_isEmpty(infoMatch[i].player1.name) && !str_isEmpty(infoMatch[i].player2.name)){
				logInFile(LOOP_FILE_NAME,"-------------------------------------------------------------------------");
				logInfo(LOOP_FILE_NAME, "-----> [%s] vs. [%s] <-----", infoMatch[i].player1.name , infoMatch[i].player2.name);
				logInfo(LOOP_FILE_NAME, "-----> [%s] Rescates: %d", infoMatch[i].player1.name , infoMatch[i].player1.rescues);
				logInfo(LOOP_FILE_NAME, "-----> [%s] Rescates: %d", infoMatch[i].player2.name , infoMatch[i].player2.rescues);
				logInfo(LOOP_FILE_NAME, "-----> [%s] Muertes: %d", infoMatch[i].player1.name , infoMatch[i].player1.deaths);
				logInfo(LOOP_FILE_NAME, "-----> [%s] Muertes: %d", infoMatch[i].player2.name , infoMatch[i].player2.deaths);
				logInFile(LOOP_FILE_NAME,"-------------------------------------------------------------------------");
			}
		}
	}
}
