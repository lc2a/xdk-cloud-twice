#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"
#include "retcode.h"

XDK_Retcode_E GyroInit(void);
void GyroDeinit(void);
void GyroGetData(SensorData* data, int cloud_ver);
