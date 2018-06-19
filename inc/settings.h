#include <stdio.h>
#include <stdint.h>
#include "BCDS_SDCard_Driver.h"
#include "retcode.h"
#include "logging.h"
#include "ff.h"

#ifndef SETTINGS_H_
#define SETTINGS_H_

/*Structure*/
typedef struct xdk_settings_n {
	int mqtt_server_port_1;
	int mqtt_server_port_2;
	char ssid[40];
	char passwd[40];
	char deviceid_1[40];
	char mqtt_user_1[40];
	char mqtt_passwd_1[40];
	char mqtt_clientid_1[40];
	char mqtt_topic_1[100];
	char mqtt_server_1[40];
	char deviceid_2[40];
	char mqtt_user_2[40];
	char mqtt_passwd_2[40];
	char mqtt_clientid_2[40];
	char mqtt_topic_2[100];
	char mqtt_server_2[40];
	int mqtt_subscribe;
	int mqtt_cloud_ver;
} xdk_settings;

XDK_Retcode_E settings_init(xdk_settings *settings);

#endif /* SETTINGS_H_ */
