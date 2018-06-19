#pragma once

#include "retcode.h"
#include "settings.h"

XDK_Retcode_E WiFiInit(xdk_settings *settings);
int WiFiDeinit(void);
void WiFiPrintIP(void);
