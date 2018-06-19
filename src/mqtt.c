#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "mqtt.h"
#include "MQTTClient.h"
#include <FreeRTOS.h>
#include <task.h>
#include "processcheck_task.h"

#include "config.h"
#include "retcode.h"
#include "logging.h"

#include "BCDS_BSP_Board.h"

static int MqttConnect(xdk_settings *settings);
static size_t SerializeData(NameValue* data, char* msg, xdk_settings *settings);

static Network mqttNet;
static Client mqttClient;

#define MQTT_BUFF_SIZE 1000
static uint8_t buf[MQTT_BUFF_SIZE];
static uint8_t readbuf[MQTT_BUFF_SIZE];

static uint8_t MESSAGE_ID = 0;
static bool deletePolling = false;
static uint32_t pollingPeriod = 1000;
static int32_t MQTT_YIELD_TIMEOUT = 50;

XDK_Retcode_E MqttInit(xdk_settings *settings)
{
    DEBUG_PRINT("Mqtt Init");

#ifdef CERT_CHECK
    _i32 retVal;
 	SlDateTime_t g_time;


 	g_time.sl_tm_day = 11;
 	g_time.sl_tm_mon = 12;
 	g_time.sl_tm_year = 2017;
 	g_time.sl_tm_sec = 32;
 	g_time.sl_tm_hour = 12;
 	g_time.sl_tm_min = 43;


 	retVal = sl_DevSet(SL_DEVICE_GENERAL_CONFIGURATION,
 						  SL_DEVICE_GENERAL_CONFIGURATION_DATE_TIME,
 						  sizeof(SlDateTime_t),(unsigned char *)(&g_time));


 	if (retVal != 0) {
 		DEBUG_PRINT("Error Setting Time\n\r");
 	}
#endif

    XDK_Retcode_E returnValue = XDK_RETCODE_FAILURE;
    NewNetwork(&mqttNet);

    if(XDK_RETCODE_SUCCESS == MqttConnect(settings))
    {
        DEBUG_PRINT("Mqtt Success");
        deletePolling = false;
        returnValue = XDK_RETCODE_SUCCESS;
    }
    else
    {
        ERR_PRINT("Mqtt initialization Failed");
    }

    return returnValue;
}

XDK_Retcode_E MqttReconnect(xdk_settings *settings)
{
    int rc = XDK_RETCODE_FAILURE;

    if (MqttConnect(settings) == XDK_RETCODE_SUCCESS) {
        //return XDK_RETCODE_SUCCESS;
    	rc = XDK_RETCODE_SUCCESS;
    }
    else
    {
    	DEBUG_PRINT("Restart the application");
    	BSP_Board_SoftReset();

    }

    //return XDK_RETCODE_FAILURE;
    return rc;
}

void MqttDeinit(void)
{
    DEBUG_PRINT("Mqtt disconnect!");
    mqttNet.disconnect(&mqttNet);
}

int MqttSendData(NameValue* data, xdk_settings *settings)
{
	char  topic[120] ="" ;
    MQTTMessage msg;
    int ret = -1;
    msg.id = MESSAGE_ID++;
    msg.qos = 0;
    char msgBuff[MQTT_BUFF_SIZE];
    msg.payload = msgBuff;
    msg.payloadlen = SerializeData(data, msgBuff, settings);
    switch (settings->mqtt_cloud_ver){
    	case 1:
    		strcat(strcat(strcat(topic,"/v1/"),settings->deviceid_1),"/data");
    		ret = MQTTPublish(&mqttClient, topic, &msg);
    		break;
    	case 2:
    		ret = MQTTPublish(&mqttClient, settings->mqtt_topic_2, &msg);
    		break;
    }
    DEBUG_PRINT("MSG: %s \n\r", (char *)msg.payload);
    return ret;
}

int MqttSubscribe(messageHandler callback, xdk_settings *settings)
{
	char  topic[120] ="" ;
    int ret = -1 ;
    switch (settings->mqtt_cloud_ver){
        case 1:
        	strcat(strcat(strcat(topic,"/v1/"),settings->deviceid_1),"/commands");
        	ret = MQTTSubscribe(&mqttClient, topic, QOS1, callback);
        	break;
        case 2:
        	strcat(strcat(strcat(topic,"/devices/"),settings->deviceid_2),"/commands");
        	ret = MQTTSubscribe(&mqttClient, topic, QOS1, callback);
        	break;
    }
    DEBUG_PRINT("MqttSubscribe:: %s %d", topic, ret);
    return ret;
}

void MqttYield(void* context)
{
    // context = context;
    for (;;)
    {
        ProcessCheck_ControlFlag(xTaskGetCurrentTaskHandle());
        MQTTYield(&mqttClient, MQTT_YIELD_TIMEOUT);
        vTaskDelay(pollingPeriod);
    }
    (void)context;
}

int MqttConnect(xdk_settings *settings)
{
    int ret = FAILURE;
    SlSockSecureFiles_t SecureFiles;
    switch (settings->mqtt_cloud_ver){
       	case 2:
       		SecureFiles.secureFiles[0] = 0;
       		SecureFiles.secureFiles[1] = 0;
       		SecureFiles.secureFiles[2] = 0;
       		SecureFiles.secureFiles[3] = 0;
       		ret = TLSConnectNetwork(&mqttNet, settings->mqtt_server_2, settings->mqtt_server_port_2, &SecureFiles, SL_SO_SEC_METHOD_TLSV1_2, SL_SEC_MASK_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA, 0);
       		break;
       	case 1:
       		ret = ConnectNetwork(&mqttNet, settings->mqtt_server_1, settings->mqtt_server_port_1);
       		break;
    }

    if(XDK_RETCODE_SUCCESS == ret || ret == -453)
    {
        MQTTClient(&mqttClient, &mqttNet, 1000, buf, MQTT_BUFF_SIZE, readbuf, MQTT_BUFF_SIZE);

        MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

        switch (settings->mqtt_cloud_ver){
               	case 2:
               		data.willFlag = 0;
               		data.MQTTVersion = 3;
               		data.clientID.cstring = settings->mqtt_clientid_2;
               		data.username.cstring = settings->mqtt_user_2;
               		data.password.cstring = settings->mqtt_passwd_2;

               		data.keepAliveInterval = 10;
               		data.cleansession = 1;
               		DEBUG_PRINT("Connecting to %s %d with user: %s, ps: %s",
               				settings->mqtt_server_2,
               		        settings->mqtt_server_port_2,
               		        settings->mqtt_user_2,
               		        settings->mqtt_passwd_2);
               		break;
                case 1:
                	data.willFlag = 0;
                	data.MQTTVersion = 3;
                	data.clientID.cstring = settings->mqtt_clientid_1;
                	data.username.cstring = settings->mqtt_user_1;
                	data.password.cstring = settings->mqtt_passwd_1;

                	data.keepAliveInterval = 10;
                	data.cleansession = 1;
                	DEBUG_PRINT("Connecting to %s %d with user: %s, ps: %s",
                			settings->mqtt_server_1,
							settings->mqtt_server_port_1,
							settings->mqtt_user_1,
							settings->mqtt_passwd_1);
                	break;
        }
        ret = MQTTConnect(&mqttClient, &data);
        if(XDK_RETCODE_SUCCESS == ret)
        {
            DEBUG_PRINT("Connected %d", ret);
        }
        else
        {
            ERR_PRINT("Mqtt Connection failed!");
        }
    } else {
        ERR_PRINT("Connect network failed - %d", ret);
    }
    return ret;
}

static size_t SerializeData(NameValue* data, char* msg, xdk_settings *settings)
{
	switch (settings->mqtt_cloud_ver){
		case 1:
			return sprintf(msg,
		           "{\"meaning\":\"%s\",\"path\":\"\",\"value\":%s}",
		           data->name,
		           data->value);
			break;

		case 2:
			return sprintf(msg,
                   "[{\"name\":\"%s\",\"value\":%s}]",
                   data->name,
                   data->value);
			break;
	}
}

void MqttStopPolling(void)
{
    deletePolling = true;
}

void MqttSetPollingPeriod(uint32_t period)
{
    pollingPeriod = period;
}

