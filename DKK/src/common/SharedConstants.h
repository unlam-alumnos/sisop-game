/*
 * SharedConstants.h
 *
 *  Created on: Nov 7, 2014
 *      Author: cristianmiranda
 */

#ifndef COMMON_SHAREDCONSTANTS_H_
#define COMMON_SHAREDCONSTANTS_H_

/*
 * Semáforos
 */
#define SEM_MONITOR_STARTED "/semMonitorStarted"
#define SEM_SYNC_DATA_MONITOR "/semSyncDataMonitorAsign"
#define SEM_SYNC_MATCH "/semGenMatch"
#define SEM_SYNC_MONITOR "/semSyncMonitor"

/*
 * Memoria compartida
 */
#define SM_RESOURCES_SPACE "/smInfoMonitor"
#define SM_MATCHES_NAME "/smMatches"
#define SM_MATCHES_PIDS_NAME "/smMatchesPids"
#define SM_MONITOR_STARTED "/smMonitorStarted"
#define SM_SYNC_DATA_MONITOR "/smSyncDataMonitorAsign"
#define SM_IS_SERVER_PRESENT "/smIsServerPresent"
#define SM_IS_CLIENT_PRESENT "/smIsClientPresent"

#endif /* COMMON_SHAREDCONSTANTS_H_ */
