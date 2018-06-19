#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"
#include "retcode.h"


XDK_Retcode_E EnvSensorInit(void);
void EnvSensorDeinit(void);
void EnvSensorGetData(SensorData* data);
