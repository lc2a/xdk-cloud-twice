#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "controls.h"
#include "leds.h"
#include "jsmn.h"
#include "MQTTClient.h"
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include "tasks.h"
#include "logging.h"
#include "mqtt.h"

#define MAX_TOKENS 20


typedef void(*CommandHandle)(const char* json, jsmntok_t* path, jsmntok_t* val);

typedef struct {
	CommandHandle cmdFn;
	char *name;
} SubscribeCmd;

static void LedControl(const char* json, jsmntok_t* path, jsmntok_t* val);
static void PeriodControl(const char* json, jsmntok_t* command, jsmntok_t* val);

static SubscribeCmd subcmds[] = {
		{
				.cmdFn = &LedControl,
				.name = "LED"
		},
		{
				.cmdFn = &PeriodControl,
				.name = "period"
		}
};

static jsmn_parser parser;
static jsmntok_t tokens[MAX_TOKENS];

static bool IsName(const char* expectedName, const char* name, int len)
{
    return (0 == strncmp(expectedName, name, len));
}

void CommandConfigHandler(MessageData* data)
{
    char* commandJson = (char*)data->message->payload;
    const int payloadLen = data->message->payloadlen;
    commandJson[data->message->payloadlen] = 0;
    jsmn_init(&parser);
    int numTokens = jsmn_parse(&parser, commandJson, payloadLen, tokens, MAX_TOKENS);

    int nameToken = -1;
    int pathToken = -1;
    int valueToken = -1;
    // token 0 is the command JSON object
    // token names are odd, values are even

    TRACE_PRINT("received json: %s", commandJson);
    for(int tokIdx = 1; tokIdx < numTokens; tokIdx += 2)
    {
        if(IsName("name", commandJson + tokens[tokIdx].start, 4))
        {
            nameToken = tokIdx;
        }
        else if(IsName("path", commandJson + tokens[tokIdx].start, 4))
        {
        	pathToken = tokIdx;
        }
        else if(IsName("value", commandJson + tokens[tokIdx].start, 5))
        {
            valueToken = tokIdx;
        }
    }

    if ((nameToken != -1) && (pathToken != -1) && (valueToken != -1))
    {
        for (unsigned int i = 0; i < sizeof(subcmds)/sizeof(SubscribeCmd); i++)
        {
            if (IsName(subcmds[i].name, commandJson + tokens[nameToken+1].start, strlen(subcmds[i].name) ))
            {
                subcmds[i].cmdFn(commandJson, tokens + pathToken +1 , tokens + valueToken +1);
            }
        }
    }

}

static void LedControl(const char* json, jsmntok_t* path, jsmntok_t* val)
{
	uint32_t op = BSP_LED_COMMAND_OFF;

    int value = -1;
    sscanf(json + val->start, "%d", &value);

    if(1 == value)
    {
        op = BSP_LED_COMMAND_ON;
    }
    else if(2 == value)
    {
        op = BSP_LED_COMMAND_TOGGLE ;
    }
    else if(0 != value)
    {
        WARN_PRINT("Unknown led op = %d", value);
        return;
    }

    int ledidx = NUM_LEDS;

    if(IsName("red", json + path->start, 3))
    {
        ledidx = RED_LED;
    }
    else if(IsName("yellow", json + path->start, 6))
    {
        ledidx = YELLOW_LED;
    }
    else if(IsName("orange", json + path->start, 6))
    {
        ledidx = ORANGE_LED;
    }
    else
    {
        WARN_PRINT("Unknown led color = %s", json + path->start);
        return;
    }

    LedSet(ledidx, op);
}

void PeriodControl(const char* json, jsmntok_t* command, jsmntok_t* val)
{
    int value = -1;
    sscanf(json + val->start, "%d", &value);

    if(0 < value)
    {
        if(IsName("publish", json + command->start, 7))
        {
            DEBUG_PRINT("publish period %d", value);
            Tasks_PublishPeriod(value * 1000);
        }
        else if(IsName("subscribe", json + command->start, 9))
        {
            MqttSetPollingPeriod((uint32_t)value);
        }
        else
        {
            WARN_PRINT("Unknown period = %d", value);
        }
    }
    else
    {
        WARN_PRINT("Wrong meas period = %d", value);
    }
}

