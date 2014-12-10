//============================================================================
// Name        : DKKClient.cpp
// Author      : Cristian Miranda
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "DKKClient.h"

#include "../../DKK/src/utils/log/LogUtils.h"
#include "../../DKK/src/utils/file/FileUtils.c"

Timer fps;
Map map;
Kong kong;
Mario pMario;
Luigi pLuigi;
Tarro tarro;
Princesa princess;
Barril barrel[BARRILES]; //= (Barril*) malloc(sizeof(Barril) * BARRILES);
Fuego flame[FUEGOS];

void construct();
void move();
void show();
void connect();

void *startMonitor(void*);
void *listenToMonitor(void*);
void launchMonitor();
void *initClient(void *);

struct sResourceClient *infoLocalClients;

sem_t *semMonitorStarted;

int *isClientPresent;
int *isServerPresent;
char *serverHost;
char *serverPort;
char *monitorPath;
char *monitorPort;

int p = 0;
char pChar[2];
char name[45];

int main( int argc, char* args[] ) {

	logInFile(LOG_FILE_NAME,"Leyendo archivo de propiedades");
	serverHost = getProperty(PROPERTIES_FILE_NAME, "server.host");
	serverPort = getProperty(PROPERTIES_FILE_NAME, "server.port");
	monitorPath = getProperty(PROPERTIES_FILE_NAME, "monitor.path");
	monitorPort = getProperty(PROPERTIES_FILE_NAME, "monitor.port");

	pthread_t threadInit;
	if( pthread_create( &threadInit , NULL ,  initClient , (void*) 0 ) < 0)
		logInFile(ERROR_FILE_NAME,"Error al crear thread que ejecuta initClient()");

	pthread_t threadMonitor;
	if( pthread_create( &threadMonitor , NULL ,  startMonitor , (void*) 0 ) < 0)
		logInFile(ERROR_FILE_NAME,"Error al crear thread que ejecuta startMonitor()");
	
    if (pthread_join(threadInit, NULL)) {
    	logInfo(ERROR_FILE_NAME, "[%s] - Error al hacer joing del thread que ejecuta initClient()", name);
	}

	if (pthread_join(threadMonitor, NULL)) {
    	logInfo(ERROR_FILE_NAME, "[%s] - Error al hacer joing del thread que ejecuta startMonitor()", name);
	}

    return 0;

}

void *initClient(void *p){

	logInFile(LOG_FILE_NAME,"Iniciando cliente");

	//Bandera de salida
    bool quit = false;

    printf("Ingrese el nombre del jugador: ");
    scanf("%s", name);
    fflush(stdin);

    // Connect with match server
    connect();

    // Construct screen
    construct();

    //While the user hasn't quit
    while( quit == false ) {
    	//Start the frame timer
		fps.start();

        //While there's an event to handle
        while( SDL_PollEvent( &event ) ) {
        	//Dejo que el handler maneje el evento
			if(p == 0){
				pMario.handle_input();
			} else {
				pLuigi.handle_input();
			}
            //If the user has Xed out the window
            if( event.type == SDL_QUIT ) {
                //Quit the program
                quit = true;
            }

        }

        // Apply movements
        move();

        // Apply changes in screen
		show();

    }

    free(walls);
    free(ladders);
    free(barrels);
    free(flames);

    //Free the surface and quit SDL
    clean_up();
}

void construct(){

	//Initialize core
	logInfo(LOG_FILE_NAME, "[%s] - Iniciando gráficos...", name);
	init();

	//Load the files
	logInfo(LOG_FILE_NAME, "[%s] - Cargando imágenes y fuentes...", name);
	load_files();

	//Clip the sprite sheet
	logInfo(LOG_FILE_NAME, "[%s] - Iniciando sprites...", name);
	set_clips();

	//Cargo los nombres de los jugadores
	logInfo(LOG_FILE_NAME, "[%s] - Construyendo los nombres de los jugadores...", name);
	player1name = TTF_RenderText_Solid( font, pMario.name, textColor );
	player2name = TTF_RenderText_Solid( font, pLuigi.name, textColor );

	//Apply the surface to the screen
	logInfo(LOG_FILE_NAME, "[%s] - Iniciando mapa...", name);
	map.show();
	apply_surface( 10, 10, player1name, screen, NULL);
	apply_surface( 330, 10, player2name, screen, NULL);

	logInfo(LOG_FILE_NAME, "[%s] - Iniciando a Mario y a Luigi...", name);
	pMario.pj = mario;
	pLuigi.pj = luigi;

}

void move(){
    pMario.move();
    pLuigi.move();
    barrel[0].move(0);
    barrel[1].move(1);
    flame[0].move(0);
    flame[1].move(1);
    if(pMario.status == MARIO_HAPPY || pLuigi.status == MARIO_HAPPY ){
    	princess.status = PRINCESA_HAPPY;
    }
    princess.move();
    kong.move();
}

void show(){
	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0, 0, 0) );
	apply_surface( 0, 0, background, screen, NULL);
	apply_surface( 10, 10, player1name, screen, NULL);
	apply_surface( 330, 10, player2name, screen, NULL);
	// Muestro los barriles
	apply_surface(SCREEN_WIDTH / 2 - BARRILESW*2, FLOOR - BARRILESH - 306, barriles, screen, &barrilesclip[0] );
	pMario.show();
	pLuigi.show();
	barrel[0].show(0);
	barrel[1].show(1);
	flame[0].show(0);
	flame[1].show(1);
	princess.show();
	kong.show();
	int marca = 0;
	if(SDL_GetTicks() > 10000 && SDL_GetTicks()<= 10500){
		if(marca == 0){
			kong.throwRight();
			barrel[0].init(0, DIR_RIGHT);
			flame[0].init(0, DIR_RIGHT);
			marca = 1;
		}else{
			marca = 0;
		}
	} else if (SDL_GetTicks() > 20000 && SDL_GetTicks() <= 20500){
		if(marca == 0){
			kong.throwLeft();
			barrel[1].init(1, DIR_LEFT);
			flame[1].init(1, DIR_LEFT);
			marca = 1;
		}else{
			marca = 0;
		}
	}
	tarro.show();

	//Update the screen
	SDL_Flip(screen);

	//Cap the frame rate
	if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
		SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
}

void connect(){
	int port = 9999;

	// Variables para la conexion
	int sock;
	struct sockaddr_in server;

	// Crea el socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
		logInfo(ERROR_FILE_NAME, "[%s] - No se pudo crear el SOCKET para la conexión con el servidor", name);

	// Preparo la estructura sockaddr_in
	server.sin_addr.s_addr = inet_addr(serverHost);
	server.sin_family = AF_INET;
	server.sin_port = htons( port );

	// Intenta conectarse al servidor
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
		logInfo(ERROR_FILE_NAME, "[%s] - No se pudo establecer conexión con el servidor", name);

	if(recv( sock , pChar , sizeof(char)*2, 0) < 0 )
		logInfo(ERROR_FILE_NAME, "[%s] - Error recibiendo ID del jugador", name);

	p = atoi(pChar);

	char aux[45];
	if(p == 0){
		pMario.isClient = true;
		pLuigi.isClient = false;
		strcpy(pMario.name, name);
		send( sock , name , sizeof(char) * 45, 0);
		if(recv( sock , aux , sizeof(char)*45, 0) < 0 ){
			logInfo(ERROR_FILE_NAME, "[%s] - Error recibiendo nombre del jugador", name);
		}else{
			strcpy(pLuigi.name,aux);
		}
	} else {
		pMario.isClient = false;
		pLuigi.isClient = true;
		strcpy(pLuigi.name,name);
		send( sock , name , sizeof(char) * 45, 0);
		if(recv( sock , aux , sizeof(char)*45, 0) < 0 ){
			logInfo(ERROR_FILE_NAME, "[%s] - Error recibiendo nombre del jugador", name);
		}else{
			strcpy(pMario.name,aux);
		}
	}

	// Test if the socket is in non-blocking mode:
	if(fcntl(sock, F_GETFL) & O_NONBLOCK) {
	    // socket is non-blocking
	} else {
		// Put the socket in non-blocking mode:
		if(fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0) {
			// handle error
		}
	}

	pMario.socket = sock;
	pLuigi.socket = sock;

	// Muestro las reglas de juego
	logInfo(LOG_FILE_NAME, "[%s] - Cliente conectado a %s:%d", name, serverHost, port);

}

void *startMonitor(void*){

	int semFd = shm_open(SM_MONITOR_STARTED, O_CREAT | O_RDWR, 0644);
	if (semFd == -1)
		logInfo(ERROR_FILE_NAME,"[%s] - ERROR shm_open() para SM_MONITOR_STARTED en startMonitor()", name);

	if (ftruncate(semFd, sizeof(sem_t)) == -1)
		logInfo(ERROR_FILE_NAME,"[%s] - ERROR ftruncate() para SM_MONITOR_STARTED en startMonitor()", name);

	semMonitorStarted = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, semFd, 0);
	if (semMonitorStarted == MAP_FAILED)
		logInfo(ERROR_FILE_NAME,"[%s] - ERROR mmap() para SEM_MONITOR_STARTED en startMonitor()", name);

	semMonitorStarted = sem_open (SEM_MONITOR_STARTED, O_CREAT, 0644, 1);

	int fd = shm_open(SM_IS_SERVER_PRESENT, O_CREAT | O_RDWR, 0644);
    if (fd == -1)
        logInFile(ERROR_FILE_NAME,"ERROR shm_open() para SM_IS_SERVER_PRESENT en startMonitor()");

    if (ftruncate(fd, sizeof(sem_t)) == -1)
        logInFile(ERROR_FILE_NAME,"ERROR ftruncate() para SM_IS_SERVER_PRESENT en startMonitor()");

    isServerPresent = (int *)malloc(sizeof(int));
    
    isServerPresent = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (isServerPresent == MAP_FAILED)
        logInFile(ERROR_FILE_NAME,"ERROR mmap() para SM_IS_SERVER_PRESENT en startMonitor()");

    int fd2 = shm_open(SM_IS_CLIENT_PRESENT, O_CREAT | O_RDWR, 0644);
    if (fd2 == -1)
        logInFile(ERROR_FILE_NAME,"ERROR shm_open() para SM_IS_CLIENT_PRESENT en startMonitor()");

    if (ftruncate(fd2, sizeof(sem_t)) == -1)
        logInFile(ERROR_FILE_NAME,"ERROR ftruncate() para SM_IS_CLIENT_PRESENT en startMonitor()");

    isClientPresent = (int *)malloc(sizeof(int));
    
    isClientPresent = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    if (isServerPresent == MAP_FAILED)
        logInFile(ERROR_FILE_NAME,"ERROR mmap() para SM_IS_CLIENT_PRESENT en startMonitor()");

	launchMonitor();
}

void launchMonitor(){
		
	sem_wait(semMonitorStarted);

	if(*isServerPresent != 1 && *isClientPresent != 1 ){		

		*isClientPresent = 1;

		logInfo(LOG_FILE_NAME, "[%s] - ANTES Iniciando Monitor en %s", name, monitorPath);


		logInfo(LOG_FILE_NAME, "[%s] - Iniciando Monitor en %s", name, monitorPath);

		pid_t pid = fork();
	    if (pid == -1) {
	    	logInfo(ERROR_FILE_NAME, "[%s] - ERROR fork() para launchMonitor", name);
	    } else if (pid) {
	    	wait(NULL);
	    	pthread_t threadCheck;
	        if( pthread_create( &threadCheck , NULL ,  listenToMonitor , (void*) 0 ) < 0)
	        	logInfo(ERROR_FILE_NAME, "[%s] - No se pudo crear el thread que ejecuta listenToMonitor", name);
	        if (pthread_join(threadCheck, NULL))
	        	logInfo(ERROR_FILE_NAME, "[%s] - No se pudo hacer join del thread que ejecuta listenToMonitor", name);
	    } else {
	    	if(execv(monitorPath, NULL) < 0)
	    		logInfo(ERROR_FILE_NAME, "[%s] - Error iniciando Monitor en launchMonitor() - execv", name);
	    }
	}else if(*isServerPresent != 1){
		sem_post(semMonitorStarted);
	}
}

void *listenToMonitor(void *){
	
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
                    *isClientPresent = 0;
                    sem_post(semMonitorStarted);    
					launchMonitor();
					break;
				}
			} else {
				logInFile(LOG_FILE_NAME,"[Monitor] - No se pudo conectar al server Monitor. Relanzando servicio.");
				*isClientPresent = 0;
				sem_post(semMonitorStarted); 
                launchMonitor();
				break;
			}
		}
	}

}
