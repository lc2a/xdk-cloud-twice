#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"
#include "retcode.h"

XDK_Retcode_E LightInit(void);
void LightDeinit(void);
void LightGetData(SensorData* data);
