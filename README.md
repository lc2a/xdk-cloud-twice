# Bosch XDK: relayr Cloud 1.0 and 2.0 how-to

This project contains the somehow maintained code to support the [Bosch XDK-110 Development Kit](https://xdk.bosch-connectivity.com/overview)

Changes have been made to support both cloud 1 and 2, config file can include both configurations.

The system will attempt to reconnect to the broker.  If the attempt fails then the system will perform a full reset.

We also address validating the server CA when making our TLS connection.  If validating the server CA then you will need to flash the [relayr certificate](https://github.com/relayr/xdk-wifi-upgrade) to the device.  


## Installation steps

* Download and import the project


## Configure the XDK development environment

### Under project properties

* update the **Paths and Symbols** to include additional paths and preprocessor symbols.  This will help with search and code completion features. **Note:** your mileage may vary across versions as to the proper paths.

* XDK Bootloader and FOTA should have the *Enable project specific settings* option checked and Bootloader version should have **Auto** selected.

## Configure the MQTT demo application


If you want to perform server CA validation then you will need to ensure that you have [flashed the server CA](https://github.com/relayr/xdk-wifi-upgrade) to the device.  

Next, uncomment the macro **CERT_CHECK** defined in **config.h** otherwise a TLS connection will be established without verification potentially opening us up to a Man in the Middle vulnerability.

Update the **config.h** with your projects information.  See the [Cloud 2.0 Primer](https://relayr.atlassian.net/wiki/spaces/COR/pages/130583370/Cloud+2.0+HowTo#Cloud2.0HowTo-SendingMeasurements).

Depending on your needs you will also need to take into consideration the sensor data tags and sending that data into one request instead of broken out individually as indicated in the following code snippet from **envdata.c**.

```
    snprintf(data->meas[0].name,  SENSOR_NAME_SIZE,  "%s", cloud_tag[0]);
    snprintf(data->meas[0].value, SENSOR_VALUE_SIZE, "%f", meas->temperature/1000.0);

    snprintf(data->meas[1].name,  SENSOR_NAME_SIZE,  "%s", "pressure");
    snprintf(data->meas[1].value, SENSOR_VALUE_SIZE, "%f", meas->pressure/100.0);

    snprintf(data->meas[2].name,  SENSOR_NAME_SIZE,  "%s", "humidity");
    snprintf(data->meas[2].value, SENSOR_VALUE_SIZE, "%d", (int)meas->humidity
```    
## Sensors configuration

Before compiling and flashing the XDK, you can enable/disable specific sensors. Update the config.h files according to your needs :
```
// Enable Sensors.
#define USE_ENV     1 
#define USE_ACCEL   1
#define USE_LIGHT   1
#define USE_GYRO    1
#define USE_MAGNETO 1
#define USE_BUTTON  1
```

## Flash the XDK-100

When flashing the XDK if you receive an error message indicating that you have an **Invalid Application** and your bootloader version is 1.1.0 then you will need to select "Add XDK Nature".  Using Workbench on a Mac you will need to right-click on the project and select -> Configure -> Add XDK Nature.

## Use of cloud 1 or cloud 2
config.h file should be filled with cloud 1 and cloud 2 configuration. If option SD_CARD_CONF is enabled, XDK.cfg file should be filled too.
The use of cloud 1 or cloud 2 is defined by the option CLOUD_VER=2, once both configuration (cloud 1 and 2) has been set up in config file, it is the only parameter to be set for switching from cloud 1 to 2.

## Use of SD-CARD configuration

SD-CARD option can be used to configured connection to cloud. 

### Configuration file and SD CARD
SD-CARD need to be formated in FAT. A file called XDK.cfg need to be place in it.
The `SD_CARD_CONF` key needs to be set to 1. 

### XDK.cfg example

```
CLOUD_VER=2
WLAN_CONNECT_WPA_SSID=SSID
WLAN_CONNECT_WPA_PASS=PASSWORD
MQTT_SUBSCRIBE=FALSE

//Credentials Cloud 1
MQTT_SERVER_PORT_1=1883
DEVICE_ID_1=device_id_cloud_1
MQTT_USER_1=mqtt_user_cloud_1
MQTT_PASSWORD_1=mqtt_password_cloud_1
MQTT_CLIENTID_1=mqtt_clientid_cloud_1
MQTT_TOPIC_1=/v1/device_id_cloud_1/data
MQTT_SERVER_1=mqtt.relayr.io

//Credential Cloud 2
MQTT_SERVER_PORT_2=8883
DEVICE_ID_2=device_id_cloud_2
MQTT_USER_2=mqtt_user_cloud_2
MQTT_PASSWORD_2=mqtt_password_cloud_2
MQTT_CLIENTID_2=mqtt_clientid_cloud_2
MQTT_TOPIC_2=devices/device_id_cloud_2/measurements
MQTT_SERVER_2=cloud-mqtt.relayr.io
```

## Json XDK Device Model
```
{
	"measurements":[
		{
			"name":"pressure",
			"type":"number",
			"min":0,
			"max":2000,
			"unit":"Pa"
		},
		{
			"name":"temperature",
			"type":"number",
			"min":-20,
			"max":100,
			"unit":"C°"
		},
		{
			"name":"humidity",
			"type":"number",
			"min":0,
			"max":100,
			"unit":"%"
		},
		{
			"name":"luminosity",
			"type":"number",
			"min":0,
			"max":10000,
			"unit":"lux"
		},
		{
			"name":"magneticField_x",
			"type":"number",
			"min":-1500,
			"max":1500,
			"unit":"uT"
		},
		{
			"name":"magneticField_y",
			"type":"number",
			"min":-1500,
			"max":1500,
			"unit":"uT"
		},
		{
			"name":"magneticField_z",
			"type":"number",
			"min":-1500,
			"max":1500,
			"unit":"uT"
		},
		{
			"name":"acceleration_x",
			"type":"number",
			"min":-16,
			"max":16,
			"unit":"g"
		},
		{
			"name":"acceleration_y",
			"type":"number",
			"min":-16,
			"max":16,
			"unit":"g"
		},
		{
			"name":"acceleration_z",
			"type":"number",
			"min":-16,
			"max":16,
			"unit":"g"
		},
		{
			"name":"angularSpeed_x",
			"type":"number",
			"min":-2000,
			"max":2000,
			"unit":"°/s"
		},
		{
			"name":"angularSpeed_y",
			"type":"number",
			"min":-2000,
			"max":2000,
			"unit":"°/s"
		},
		{
			"name":"angularSpeed_z",
			"type":"number",
			"min":-2000,
			"max":2000,
			"unit":"°/s"
		},
		{
			"name":"Button_1_state",
			"displayName":"",
			"type":"boolean",
			"unit":""
		},
		{
			"name":"Button_2_state",
			"displayName":"",
			"type":"boolean",
			"unit":""
		},
	]
}
```
