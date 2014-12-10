/*
 * FileUtils.c
 *
 *  Created on: Oct 30, 2014
 *      Author: cristianmiranda
 */

#include "FileUtils.h"

#include "../string/StringUtils.h"

char* getPropertyValue(char *fileName, char *key){
	FILE *fp = fopen(fileName, "r");
	char string[255];
	while(!feof(fp)) {
	    if (fgets(string,2000,fp)) {
	    	if(!str_isEmpty(string) && !str_contains(string, "#")){
	    		char** pair = str_split(string, '=');
	    		if (pair && !strcmp(*(pair), key)) {
					fclose(fp);
					return *(pair + 1);
				}
	    	}
	    }
	}
	fclose(fp);
	return NULL;
}

char* getProperty(char *fileName, char *key){
	char *result = getPropertyValue(fileName, key);
	char *value = (char*) malloc(sizeof(char) * strlen(result));
	strcpy(value, result);
	// Eliminando caracteres de nueva linea y retorno de carro
	size_t ln = strlen(value) - 1;
	if (value[ln] == '\n' || value[ln] == '\r')
	    value[ln] = '\0';
	free(result);
	return value;
}
