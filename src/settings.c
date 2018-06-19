#include "../inc/settings.h"

#include "BCDS_SDCard_Driver.h"
#include "diskio.h"
#include "config.h"
#include "settings.h"

#define DEFAULT_LOGICAL_DRIVE     ""
#define DRIVE_ZERO  UINT8_C(0)
#define FORCE_MOUNT  UINT8_C(1)
#define FIRST_LOCATION  UINT8_C(0)

char *  LTRIM(char *p) //Remove leading white-space characters from the string, XDK Proximetry code
{
	if (NULL != p)
		while (isspace(*p))
			++p;
	return p;
}

char *RTRIM(char *p) //Remove trailing white-space characters from the string, XDK  Proximetry code
{
	if ((NULL != p) && ('\0' != *p))
	{
		char *rp = p + strlen(p) - 1;
		while (isspace(*rp) && (rp >= p))
			--rp;
		rp[1] = '\0';
	}
	return p;
}

char *TRIM(char *p) // Remove both leading and trailing white-space characters from the string, XDK Proximetry code
{
	return RTRIM(LTRIM(p));
}


void Sd_Card_init(xdk_settings *settings)
{
	const char Filename[] ="XDK.cfg";
	Retcode_T retVal = RETCODE_FAILURE;
	FRESULT	 FileSystemResult = FR_OK;
	FILINFO	fileInfo;
	static	FATFS FatFileSystemObject;
	static	FIL	fileObject;
	char	ramBufferRead[512];	// Temporary buffer for read file
	char *endptr ;
	char *key ;
	char *value ;

	SDCardDriver_Initialize();
	if(SDCARD_INSERTED == SDCardDriver_GetDetectStatus()){
		retVal = SDCardDriver_DiskInitialize(DRIVE_ZERO);
		if(RETCODE_OK == retVal){
			printf("SD Card Disk initialize succeeded \n\r");
			FileSystemResult = f_mount(&FatFileSystemObject, DEFAULT_LOGICAL_DRIVE, FORCE_MOUNT);
			if(FR_OK != FileSystemResult){
				printf("Mounting SD card failed \n\r");
			}
			else {
				DEBUG_PRINT("Mounting SD card succeeded \n\r");
				DEBUG_PRINT("Filename : %s \n",Filename);
				DEBUG_PRINT("Return code : %d \n",f_stat(Filename,&fileInfo));
				if ( FR_OK == f_stat(Filename,NULL)){
					DEBUG_PRINT("File found !! \n\r");
					printf ("Read data from file %s of the SD card \n\r",Filename);
					f_open(&fileObject, Filename, FA_OPEN_EXISTING | FA_READ);
					f_lseek(&fileObject, FIRST_LOCATION);
					while (f_gets(ramBufferRead,sizeof(ramBufferRead),&fileObject) != NULL) //parsing config file, pieces of XDK Proximetry code
					{
						endptr = strstr(ramBufferRead, "//"); //delete comments
						if (endptr != NULL){
							*endptr='\0';
						}
						endptr = strchr(ramBufferRead, '='); //split chain with = char
						if (endptr != NULL){ //fill settings structure with values from config file , pieces of Proximetry code
							*endptr='\0';
							key = TRIM(ramBufferRead);
							DEBUG_PRINT("Key %s \n",key);
						    value = TRIM(endptr+1);
						    DEBUG_PRINT(" Value %s \n",value);
						    if (!strcmp(key,"MQTT_SERVER_PORT_1")){
						    	settings->mqtt_server_port_1=atoi(value);
						    }
						    if (!strcmp(key,"MQTT_SERVER_PORT_2")){
						    	settings->mqtt_server_port_2=atoi(value);
						    }
						    if (!strcmp(key,"WLAN_CONNECT_WPA_SSID")){
						    	strncpy(settings->ssid, value, sizeof(settings->ssid));
						    }
						    if (!strcmp(key,"WLAN_CONNECT_WPA_PASS")){
						    	strncpy(settings->passwd, value, sizeof(settings->passwd));
						    }
						    if (!strcmp(key,"DEVICE_ID_1")){
						    	strncpy(settings->deviceid_1, value, sizeof(settings->deviceid_1));
						    }
						    if (!strcmp(key,"DEVICE_ID_2")){
						    	strncpy(settings->deviceid_2, value, sizeof(settings->deviceid_2));
						    }
						    if (!strcmp(key,"MQTT_USER_1")){
						    	strncpy(settings->mqtt_user_1, value, sizeof(settings->mqtt_user_1));
						    }
						    if (!strcmp(key,"MQTT_USER_2")){
						    	strncpy(settings->mqtt_user_2, value, sizeof(settings->mqtt_user_2));
						    }
						    if (!strcmp(key,"MQTT_PASSWORD_1")){
						    	strncpy(settings->mqtt_passwd_1, value, sizeof(settings->mqtt_passwd_1));
						    }
						    if (!strcmp(key,"MQTT_PASSWORD_2")){
						    	strncpy(settings->mqtt_passwd_2, value, sizeof(settings->mqtt_passwd_2));
						    }
						    if (!strcmp(key,"MQTT_CLIENTID_1")){
						    	strncpy(settings->mqtt_clientid_1, value, sizeof(settings->mqtt_clientid_1));
						    }
						    if (!strcmp(key,"MQTT_CLIENTID_2")){
						    	strncpy(settings->mqtt_clientid_2, value, sizeof(settings->mqtt_clientid_2));
						    }
						    if (!strcmp(key,"MQTT_TOPIC_1")){
						    	strncpy(settings->mqtt_topic_1, value, sizeof(settings->mqtt_topic_1));
						    }
						    if (!strcmp(key,"MQTT_TOPIC_2")){
						    	strncpy(settings->mqtt_topic_2, value, sizeof(settings->mqtt_topic_2));
						    }
						    if (!strcmp(key,"MQTT_SERVER_1")){
						    	strncpy(settings->mqtt_server_1, value, sizeof(settings->mqtt_server_1));
						    }
						    if (!strcmp(key,"MQTT_SERVER_2")){
						    	strncpy(settings->mqtt_server_2, value, sizeof(settings->mqtt_server_2));
						    }
						    if (!strcmp(key,"MQTT_SUBSCRIBE")){
						    	settings->mqtt_subscribe=atoi(value);
						    }
						    if (!strcmp(key,"CLOUD_VER")){
						    	settings->mqtt_cloud_ver=atoi(value);
						    }
						}
			        }
			        f_close(&fileObject);
				}
				else {
					DEBUG_PRINT("Conf File not found !! \n\r");
				}
			}
		}
	}
}

XDK_Retcode_E settings_init(xdk_settings *settings)
{
	if (SD_CARD_CONF){ //If read parameters from SDCARD enable read
		Sd_Card_init(settings);
	}
	else { //If read parameters from SDCARD Disable, populate struct settings with credentials.h
		settings->mqtt_server_port_1=MQTT_SERVER_PORT_1;
		settings->mqtt_server_port_2=MQTT_SERVER_PORT_2;
		strncpy(settings->ssid, WLAN_CONNECT_WPA_SSID, sizeof(settings->ssid));
		strncpy(settings->passwd, WLAN_CONNECT_WPA_PASS, sizeof(settings->passwd));
		strncpy(settings->deviceid_1, DEVICE_ID_1, sizeof(settings->deviceid_1));
		strncpy(settings->deviceid_2, DEVICE_ID_2, sizeof(settings->deviceid_2));
		strncpy(settings->mqtt_user_1, MQTT_USER_1, sizeof(settings->mqtt_user_1));
		strncpy(settings->mqtt_user_2, MQTT_USER_2, sizeof(settings->mqtt_user_2));
		strncpy(settings->mqtt_passwd_1, MQTT_PASSWORD_1, sizeof(settings->mqtt_passwd_1));
		strncpy(settings->mqtt_passwd_2, MQTT_PASSWORD_2, sizeof(settings->mqtt_passwd_2));
		strncpy(settings->mqtt_clientid_1, MQTT_CLIENTID_1, sizeof(settings->mqtt_clientid_1));
		strncpy(settings->mqtt_clientid_2, MQTT_CLIENTID_2, sizeof(settings->mqtt_clientid_2));
		strncpy(settings->mqtt_topic_1, MQTT_TOPIC_1, sizeof(settings->mqtt_topic_1));
		strncpy(settings->mqtt_topic_2, MQTT_TOPIC_2, sizeof(settings->mqtt_topic_2));
		strncpy(settings->mqtt_server_1, MQTT_SERVER_1, sizeof(settings->mqtt_server_1));
		strncpy(settings->mqtt_server_2, MQTT_SERVER_2, sizeof(settings->mqtt_server_2));
		settings->mqtt_subscribe=MQTT_SUBSCRIBE;
		settings->mqtt_cloud_ver=CLOUD_VER;
	}

	DEBUG_PRINT(" Wlan SSID Value is %s \n",settings->ssid);
	DEBUG_PRINT("Wlan Password Value is %s \n",settings->passwd);

	DEBUG_PRINT(" Server Port 1 Value is %d \n",settings->mqtt_server_port_1);
	DEBUG_PRINT("Device ID 1 Value is %s \n",settings->deviceid_1);
	DEBUG_PRINT("MQTT USER 1 Value is %s \n",settings->mqtt_user_1);
	DEBUG_PRINT("MQTT PASSWORD 1 Value is %s \n",settings->mqtt_passwd_1);
	DEBUG_PRINT("MQTT CLIENTID 1 Value is %s \n",settings->mqtt_clientid_1);
	DEBUG_PRINT("MQTT TOPIC 1 Value is %s \n",settings->mqtt_topic_1);
	DEBUG_PRINT("MQTT SERVER  1 Value is %s \n",settings->mqtt_server_1);

	DEBUG_PRINT(" Server Port 2 Value is %d \n",settings->mqtt_server_port_2);
	DEBUG_PRINT("Device ID 2 Value is %s \n",settings->deviceid_2);
	DEBUG_PRINT("MQTT USER 2 Value is %s \n",settings->mqtt_user_2);
	DEBUG_PRINT("MQTT PASSWORD 2 Value is %s \n",settings->mqtt_passwd_2);
	DEBUG_PRINT("MQTT CLIENTID 2 Value is %s \n",settings->mqtt_clientid_2);
	DEBUG_PRINT("MQTT TOPIC 2 Value is %s \n",settings->mqtt_topic_2);
	DEBUG_PRINT("MQTT SERVER  2 Value is %s \n",settings->mqtt_server_2);

	DEBUG_PRINT("MQTT SUBSCRIBE Value is %d \n",settings->mqtt_subscribe);
	DEBUG_PRINT("MQTT CLOUD VER Value is %d \n",settings->mqtt_cloud_ver);

	if (&settings != NULL)
	{
		return XDK_RETCODE_SUCCESS;
	}
	else
	{
		DEBUG_PRINT("Unable to read parameters and populate settings structure \n");
	}
}
