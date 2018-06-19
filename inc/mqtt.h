#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"
#include "MQTTClient.h"
#include "retcode.h"
#include "settings.h"

typedef enum returnCode MqttErrorCode;

XDK_Retcode_E  MqttInit(xdk_settings *settings);
void MqttDeinit(void);
XDK_Retcode_E  MqttReconnect(xdk_settings *settings);
int  MqttSendData(NameValue* data,xdk_settings *settings);
int  MqttSubscribe(messageHandler callback, xdk_settings *settings);
void MqttYield(void* context);
void MqttStopPolling(void);
void MqttSetPollingPeriod(uint32_t period);
