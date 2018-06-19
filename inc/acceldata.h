#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"
#include "retcode.h"


XDK_Retcode_E AccelInit(void);
void AccelDeinit(void);
void AccelGetData(SensorData* data, int cloud_ver);
