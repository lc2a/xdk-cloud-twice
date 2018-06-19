#pragma once

#include <stdbool.h>
#include "BCDS_SensorErrorType.h"
#include "retcode.h"

#define MAX_MEASUREMENTS  (  7 )
#define NUM_SENSORS       (  7 )
#define SENSOR_NAME_SIZE  ( 20 )
#define SENSOR_VALUE_SIZE ( 300 )

typedef struct
{
    char name[SENSOR_NAME_SIZE];
    char value[SENSOR_VALUE_SIZE];
} NameValue;

typedef struct
{
    NameValue meas[MAX_MEASUREMENTS];
    uint32_t numMeas;
} SensorData;

typedef void(*SensorGetter)(SensorData* data, int cloud_ver);
typedef Retcode_T(*SensorInitializer)(void* handle);

extern SensorGetter sensors[];
extern bool enabledSensors[];

XDK_Retcode_E SensorInit(SensorInitializer init, void* handle, const char* label);
void SensorDataClear(SensorData* data);
