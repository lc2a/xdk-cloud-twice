#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>

#include "PIp.h"
#include "Serval_Ip.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
#include <FreeRTOS.h>

#ifdef WLANCONNECT_ENROLLEE_PIN
# include <XdkVersion.h>
#endif

#include "em_wdog.h"
#include "logging.h"
#include "retcode.h"

#include "config.h"

static int wifiInitDone = 0;

XDK_Retcode_E WiFiInit(xdk_settings *settings)
{
    WDOG_Feed();

    DEBUG_PRINT("WiFi Init");

    XDK_Retcode_E retVal = XDK_RETCODE_FAILURE;

#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR <= 6)
    NCI_ipSettings_t myIpSettings;
#else
    NetworkConfig_IpSettings_T myIpSettings;
#endif

    memset(&myIpSettings, (uint32_t) 0, sizeof(myIpSettings));
    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();
    int8_t ipAddressMy[15] = {0};

#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR <= 6)
    WLI_connectSSID_t connectSSID;
    WLI_connectPassPhrase_t connectPassPhrase;
    if(0 == wifiInitDone && WLI_SUCCESS != WLI_init())
#else
    WlanConnect_SSID_T connectSSID;
    WlanConnect_PassPhrase_T connectPassPhrase;
    if(0 == wifiInitDone && RETCODE_OK != WlanConnect_Init())
#endif
    {
        ERR_PRINT("Error occurred initializing WLAN");
        return XDK_RETCODE_FAILURE;
    }

    wifiInitDone = 1;

    DEBUG_PRINT("Connecting to: %s", settings->ssid);

#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR <= 6)
    connectSSID = (WLI_connectSSID_t)settings->ssid;
    connectPassPhrase = (WLI_connectPassPhrase_t)settings->passwd;
#else
    connectSSID = (WlanConnect_SSID_T)settings->ssid;
    connectPassPhrase = (WlanConnect_PassPhrase_T) settings->passwd;
#endif

#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR <= 6)
    if (WLI_SUCCESS == WLI_connectWPA(connectSSID, connectPassPhrase, NULL))
    {
        NCI_getIpSettings(&myIpSettings);
#else
    if (RETCODE_OK == WlanConnect_WPA(connectSSID, connectPassPhrase, NULL))
    {
            NetworkConfig_GetIpSettings(&myIpSettings);
#endif
        WDOG_Feed();
        *IpaddressHex = Basics_htonl(myIpSettings.ipV4);
        (void)Ip_convertAddrToString(IpaddressHex,(char *)&ipAddressMy);
        DEBUG_PRINT("Connected - Ip address of the device: %s",ipAddressMy);
        retVal = XDK_RETCODE_SUCCESS;
    }
    else
    {
        ERR_PRINT("Error occurred connecting %s",settings->ssid);
    }

    return retVal;
}

int WiFiDeinit(void)
{
    DEBUG_PRINT("WiFi disconnect!");
#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR <= 6)
    return WLI_disconnect(NULL);
#else
    return WlanConnect_Disconnect(NULL);
#endif
}

void WiFiPrintIP(void)
{
#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR <= 6)
    NCI_ipSettings_t myIpSettings;
#else
    NetworkConfig_IpSettings_T myIpSettings;
#endif

    memset(&myIpSettings, 0, sizeof(myIpSettings));

    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();

#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR <= 6)
    NCI_getIpSettings(&myIpSettings);
#else
    NetworkConfig_GetIpSettings(&myIpSettings);
#endif
    *IpaddressHex = Basics_htonl(myIpSettings.ipV4);

    int8_t ipAddressMy[15] = {0};
    (void)Ip_convertAddrToString(IpaddressHex,(char *)&ipAddressMy);

    DEBUG_PRINT("IP address of the device: %s", ipAddressMy);
}
