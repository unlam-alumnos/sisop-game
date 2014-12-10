/*
 * LogUtils.h
 *
 *  Created on: Oct 29, 2014
 *      Author: cristianmiranda
 */

#ifndef UTILS_LOG_LOGUTILS_H_
#define UTILS_LOG_LOGUTILS_H_

#include "../includes/General.h"
#include <stdarg.h>
#include <time.h>

/**
 * Log in the given file
 */
void logInFile(char *fileName, char *msg);
void logInfo(char *fileName, char *msg, ...);

#endif /* UTILS_LOG_LOGUTILS_H_ */
