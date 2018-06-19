#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"
#include "retcode.h"

XDK_Retcode_E MagnetoInit(void);
void MagnetoDeinit(void);
void MagnetoGetData(SensorData* data, int cloud_ver);
