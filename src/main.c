#include <stdio.h>
#include <stdint.h>

#include "BCDS_Basics.h"
#include <FreeRTOS.h>
#include <timers.h>

#include "../inc/settings.h"
#include "XdkSystemStartup.h"
#include "config.h"

// sensors
#include "sensordata.h"
#include "envdata.h"
#include "acceldata.h"
#include "lightdata.h"
#include "gyrodata.h"
#include "magnetodata.h"

// peripherals
#include "leds.h"
#include "wifi.h"
#include "xdkbutton.h"
#include "tasks.h"
#include "processcheck_task.h"
#include "controls.h"
#include "xdkwdog.h"
#include "retcode.h"
#include "logging.h"

#include "BCDS_UtilsConfig.h"
#include "BCDS_Assert.h"
#include "BCDS_Retcode.h"
#include "BCDS_CmdProcessor.h"
#include "task.h"

/* global variables ********************************************************* */
static CmdProcessor_T MainCmdProcessor;

/* header definition ******************************************************** */
#ifndef XDK110_XDKAPPLICATIONTEMPLATE_H_
#define XDK110_XDKAPPLICATIONTEMPLATE_H_

/* local interface declaration ********************************************** */
 /* Priorities */
#define TASK_PRIO_MAIN_CMD_PROCESSOR                (UINT32_C(1))
#define TASK_STACK_SIZE_MAIN_CMD_PROCESSOR          (UINT16_C(700))
#define TASK_Q_LEN_MAIN_CMD_PROCESSOR               (UINT32_C(10))

void appInitSystem(void * CmdProcessorHandle, uint32_t param2);

#endif /* XDK110_XDKAPPLICATIONTEMPLATE_H_ */

SensorGetter sensors[] = {&EnvSensorGetData,
                          &AccelGetData,
                          &LightGetData,
                          &GyroGetData,
                          &MagnetoGetData,
						  &ButtonGetData};

bool enabledSensors[] = {
                         USE_ENV, // Environmental Sensor
                         USE_ACCEL, // Accel Sensor
                         USE_LIGHT, // Light Sensor
                         USE_GYRO, // Gyro Sensor
                         USE_MAGNETO,  // Magneto Sensor
						 USE_BUTTON, // Button state
                        };

int main(void)
{
    /* Mapping Default Error Handling function */
    Retcode_T returnValue = Retcode_initialize(DefaultErrorHandlingFunc);
    if (RETCODE_OK == returnValue)
    {
        returnValue = systemStartup();
    }
    if (RETCODE_OK == returnValue)
    {
        returnValue = CmdProcessor_initialize(&MainCmdProcessor, (char *) "MainCmdProcessor", TASK_PRIO_MAIN_CMD_PROCESSOR, TASK_STACK_SIZE_MAIN_CMD_PROCESSOR, TASK_Q_LEN_MAIN_CMD_PROCESSOR);
    }
    if (RETCODE_OK == returnValue)
    {
        /* Here we enqueue the application initialization into the command
         * processor, such that the initialization function will be invoked
         * once the RTOS scheduler is started below.
         */
        returnValue = CmdProcessor_enqueue(&MainCmdProcessor, appInitSystem, &MainCmdProcessor, UINT32_C(0));
    }
    if (RETCODE_OK != returnValue)
    {
        printf("System Startup failed");
        assert(false);
    }
    /* start scheduler */
    vTaskStartScheduler();
}

void appInitSystem(void * CmdProcessorHandle, uint32_t param2)
{
	if (CmdProcessorHandle == NULL){
		printf("Command processor handle is null \r\n");
		assert(false);
	}
	BCDS_UNUSED(param2);

	/*Call Task Wifi Connection*/
	//if (pdPASS != xTaskCreate(wifiApplication, (const char	* const) "wifiApplication", 768, NULL, 1,0))
	//    {}

    /*Call the Application Init API*/
	LedInit();
#if USE_BUTTON
	ButtonInitialize();
#endif
#if USE_ENV
	EnvSensorInit();
#endif /* USE_ENV */
#if USE_ACCEL
	AccelInit();
#endif /* USE_ACCEL */
#if USE_LIGHT
	LightInit();
#endif /* USE_LIGHT */
#if USE_GYRO
	GyroInit();
#endif /* USE_GYRO */
#if USE_MAGNETO
	MagnetoInit();
#endif /* USE_MAGNETO */
	XdkWDog_Init();
	Tasks_init();
}
