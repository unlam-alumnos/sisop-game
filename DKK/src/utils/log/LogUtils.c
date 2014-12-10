/*
 * LogFile.c
 *
 *  Created on: Oct 29, 2014
 *      Author: cristianmiranda
 */

#include "LogUtils.h"

void logInFile(char *fileName, char *msg){
	// Calculo la hora local
	char formatted_gmt[50];
	time_t c_time = time(NULL);
	struct tm *gmt = localtime(&c_time);
	strftime ( formatted_gmt, sizeof(formatted_gmt), "[%F - %H:%M:%S]", gmt );

	// Escribo en el archivo
	FILE *file = fopen(fileName,"a");
	if (file == NULL){
		puts("No se pudo crear el archivo de log.");
	}
	fprintf(file, "%s - %s\n", formatted_gmt, msg);
	fclose(file);
}

void logInfo(char *fileName, char *msg, ...){
	char buffer[256];
	va_list argptr;
	va_start(argptr, msg);
	vsnprintf(buffer, 255, msg, argptr);
	va_end(argptr);
	logInFile(fileName, buffer);
}
