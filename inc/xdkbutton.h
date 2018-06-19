#pragma once

#include <stdint.h>
#include <stdio.h>
#include "BCDS_Retcode.h"
#include "sensordata.h"

static const int32_t BUTTONONE = 1;

bool buttonstate[2] ;

typedef void(*buttonHandler)(void*, uint32_t);

Retcode_T ButtonInitialize(void);

int IsButtonPressed(void* handle);
int IsButtonReleased(void* handle);

void ButtonGetData(SensorData* data);
