/*
 * processcheck_task.h
 *
 * Checking state of registered tasks.
 * If task doesn't set alive flag during defined period (default 30 seconds)
 * then board will be restarted by watchdog.
 *
 *  Created on: Aug 12, 2016
 *      Author: krcka
 */

#ifndef INC_PROCESSCHECK_TASK_H_
#define INC_PROCESSCHECK_TASK_H_

#include "retcode.h"
#include "FreeRTOS.h"

/**
 * It will create FreeRTOS task and
 * created task will be registered to check mechanism
 *
 */
#define TASK_CREATE_WITHCHECK(task, name, stack, priority, handle) \
                xTaskCreate(task, (const char *)name, stack, NULL, priority, &handle); \
                ProcessCheck_RegisterProc(handle, name)

/*
 * Initialization of control mechanism
 * Call it after start of the box
 *
 */
XDK_Retcode_E ProcessCheck_Init(void);

/*
 * Register task and name of the task (name is for debugging)
 */
void ProcessCheck_RegisterProc(xTaskHandle pvProc, const char * const name);

/*
 * Set the period (in seconds)
 */
void ProcessCheck_SetMaxState(xTaskHandle pvProc, int nMaxState);

/**
 * Set flag that task is still alive
 */
void ProcessCheck_ControlFlag(xTaskHandle pvProc);


#endif /* INC_PROCESSCHECK_TASK_H_ */
