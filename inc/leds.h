#pragma once

#include "BCDS_BSP_LED.h"
#include "BSP_BoardType.h"

static const int RED_LED = 0;
static const int YELLOW_LED = 1;
static const int ORANGE_LED = 2;
static const int NUM_LEDS = 3;

Retcode_T LedInit(void);
Retcode_T LedDeinit(void);
void LedSet(uint32_t led, uint32_t op);
