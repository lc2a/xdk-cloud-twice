#pragma once

#include <stdio.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>

#include "retcode.h"

XDK_Retcode_E Tasks_init(void);
void Tasks_restart(void);
void Tasks_PublishPeriod(uint32_t period);

typedef enum task_states {
    SETTINGS_INIT = 0,
    WIFI_INIT, //
    MQTT_CONN,
    MQTT_SUB,
    RUNNING_STATE,
    ERROR_STATE,
    RESTART
} TASK_STATES;
