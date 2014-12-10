/*
 * DKKTournamentServer.h
 *
 *  Created on: Oct 30, 2014
 *      Author: cristianmiranda
 */

#ifndef DKKTOURNAMENTSERVER_H_
#define DKKTOURNAMENTSERVER_H_

#include "../../DKK/src/utils/includes/General.h"
#include "../../DKK/src/utils/includes/Process.h"
#include "../../DKK/src/utils/includes/Connection.h"
#include "../../DKK/src/utils/includes/SharedMemory.h"
#include "../../DKK/src/utils/includes/Thread.h"
#include "../../DKK/src/utils/includes/Semaphore.h"
#include "../../DKK/src/utils/includes/Graphics.h"

#include "../../DKK/src/utils/structs/CommonStructs.h"
#include "../../DKK/src/common/SharedConstants.h"

#define PROPERTIES_FILE_NAME "server.conf"

#define LOG_FILE_NAME "DKKTournamentServer.log"
#define LOOP_FILE_NAME "DKKTournamentServerLoop.log"
#define ERROR_FILE_NAME "DKKTournamentServerError.log"

#endif /* DKKTOURNAMENTSERVER_H_ */
