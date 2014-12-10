/*
 * StringUtils.h
 *
 *  Created on: Oct 30, 2014
 *      Author: cristianmiranda
 */

#ifndef UTILS_STRING_STRINGUTILS_H_
#define UTILS_STRING_STRINGUTILS_H_

#include "../includes/General.h"

int str_isEmpty(char *str);

int str_equals(char *str, char *str2);

int str_contains(char *str, char *contains);

char** str_split(char* a_str, const char a_delim);

#endif /* UTILS_STRING_STRINGUTILS_H_ */
