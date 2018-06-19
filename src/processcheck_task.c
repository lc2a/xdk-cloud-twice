/*
 * processcheck_task.c
 *
 *  Created on: Aug 12, 2016
 *      Author: krcka
 */

#include <stdio.h>
#include <FreeRTOS.h>
#include <projdefs.h>
#include <task.h>

#include "retcode.h"
#include "xdkwdog.h"
#include "logging.h"

#define PROCESSCHECK_MAXPROCS   20

typedef struct {
    xTaskHandle taskHandle;
    int8_t nState;
    int8_t nMaxState;
    char name[configMAX_TASK_NAME_LEN];
} proccheck_t;

static proccheck_t abTaskFlags[PROCESSCHECK_MAXPROCS];
static xTaskHandle mainTaskHandle;

static void ProcessCheck_Task(void *context)
{

    while (1)
    {
        for (uint8_t i = 0; i < PROCESSCHECK_MAXPROCS; i++)
        {
            if (abTaskFlags[i].nState == -1)
                break;

            if (abTaskFlags[i].nState >= abTaskFlags[i].nMaxState)
            {
                FATAL_PRINT("!!!!!!!! RESTART - frozen process %s ",
                        abTaskFlags[i].name);
                vTaskDelay(2000 / portTICK_RATE_MS);
                XdkWdog_RestartByWdog();
            }
            abTaskFlags[i].nState++;
        }

        XdkWdog_Feed();
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    (void)context;
}

XDK_Retcode_E ProcessCheck_Init(void)
{
    for (uint8_t i = 0; i < PROCESSCHECK_MAXPROCS; i++)
    {
        abTaskFlags[i].taskHandle = NULL;
        abTaskFlags[i].nState = -1;
        abTaskFlags[i].nMaxState = 30;
        memset(abTaskFlags[i].name, sizeof(((proccheck_t *) 0)->name), 0);
    }

    xTaskCreate(ProcessCheck_Task, (const char * ) "ProcessCheck", 1000, NULL,
            tskIDLE_PRIORITY, &mainTaskHandle);

    if (mainTaskHandle == NULL)
    	return XDK_RETCODE_FAILURE;

    return XDK_RETCODE_SUCCESS;
}

void ProcessCheck_RegisterProc(xTaskHandle pvProc, const char * const name)
{
    for (uint8_t i = 0; i < PROCESSCHECK_MAXPROCS; i++)
    {
        if (abTaskFlags[i].nState == -1)
        {
            abTaskFlags[i].nState = 0;
            abTaskFlags[i].taskHandle = pvProc;
            strncpy(abTaskFlags[i].name, name, sizeof((proccheck_t *) 0)->name);
            abTaskFlags[i].name[sizeof(((proccheck_t *) 0)->name) - 1] = 0;
            break;
        }
    }
}

void ProcessCheck_SetMaxState(xTaskHandle pvProc, int nMaxState)
{
    for (uint8_t i = 0; i < PROCESSCHECK_MAXPROCS; i++)
    {
        if (abTaskFlags[i].taskHandle == pvProc)
        {
            abTaskFlags[i].nMaxState = nMaxState;
            break;
        }
    }
}

void ProcessCheck_ControlFlag(xTaskHandle pvProc)
{
    for (uint8_t i = 0; i < PROCESSCHECK_MAXPROCS; i++)
    {
        if (abTaskFlags[i].taskHandle == pvProc)
        {
            abTaskFlags[i].nState = 0;
            break;
        }
    }
}
