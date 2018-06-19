/*
 * config.h
 *
 *  Created on: Sep 13, 2016
 *      Author: krcka
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

// Enable for Debug mode.
#define DEBUG

//remove if your not doing a certificate check
//#define CERT_CHECK

// Enable Sensors.
#define USE_ENV     1
#define USE_ACCEL   1
#define USE_LIGHT   1
#define USE_GYRO    1
#define USE_MAGNETO 1
#define USE_BUTTON  1

// Set WLAN credentials.
#define WLAN_CONNECT_WPA_SSID   "SSID"
#define WLAN_CONNECT_WPA_PASS   "PASSWORD"

//CLOUD 2 CRED

// MQTT credentials Cloud 1.
#define DEVICE_ID_1               "DEVICE_ID_1"
#define MQTT_SERVER_PORT_1        1883
#define MQTT_USER_1               "MQTT_USER_1"
#define MQTT_PASSWORD_1           "MQTT_PASSWORD_1"
#define MQTT_CLIENTID_1           "MQTT_CLIENTID_1"
#define MQTT_TOPIC_1              "/v1/" DEVICE_ID_1 "/data"
#define MQTT_SERVER_1             "mqtt.relayr.io"

// MQTT credentials Cloud 2.
#define DEVICE_ID_2               "DEVICE_ID_2"
#define MQTT_SERVER_PORT_2        8883
#define MQTT_USER_2               "MQTT_USER_2"
#define MQTT_PASSWORD_2           "MQTT_PASSWORD_2"
#define MQTT_CLIENTID_2           "MQTT_CLIENTID_2"
#define MQTT_TOPIC_2              "devices/" DEVICE_ID_2 "/measurements"
#define MQTT_SERVER_2             "cloud-mqtt.relayr.io"

#define MQTT_SUBSCRIBE          FALSE
#define CLOUD_VER				2

// Define publishing period to the cloud.
#define PUBLISH_PERIOD          1000

// enable retrieving the configuration from the SD card instead
#define SD_CARD_CONF            1

#endif /* _CONFIG_H_ */
