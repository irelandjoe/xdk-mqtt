#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>

#include "PIp.h"
#include "Serval_Ip.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
#include <FreeRTOS.h>

#if __has_include(<XdkVersion.h>)
# include <XdkVersion.h>
#endif

#include "em_wdog.h"
#include "logging.h"
#include "retcode.h"

#include "credentials.h"

static int wifiInitDone = 0;

XDK_Retcode_E WiFiInit()
{
    WDOG_Feed();

    DEBUG_PRINT("WiFi Init");

    XDK_Retcode_E retVal = XDK_RETCODE_FAILURE;

#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR == 7)
    NetworkConfig_IpSettings_T myIpSettings;
#else
    NCI_ipSettings_t myIpSettings;
#endif

    memset(&myIpSettings, (uint32_t) 0, sizeof(myIpSettings));
    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();
    int8_t ipAddressMy[15] = {0};

#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR == 7)
    WlanConnect_SSID_T connectSSID;
    WlanConnect_PassPhrase_T connectPassPhrase;
    if(0 == wifiInitDone && RETCODE_OK != WlanConnect_Init())
#else
    WLI_connectSSID_t connectSSID;
    WLI_connectPassPhrase_t connectPassPhrase;
    if(0 == wifiInitDone && WLI_SUCCESS != WLI_init())
#endif
    {
        ERR_PRINT("Error occurred initializing WLAN");
        return XDK_RETCODE_FAILURE;
    }

    wifiInitDone = 1;

    DEBUG_PRINT("Connecting to: %s", WLAN_CONNECT_WPA_SSID);

#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR == 7)
    connectSSID = (WlanConnect_SSID_T) WLAN_CONNECT_WPA_SSID;
    connectPassPhrase = (WlanConnect_PassPhrase_T) WLAN_CONNECT_WPA_PASS;
#else
    connectSSID = (WLI_connectSSID_t) WLAN_CONNECT_WPA_SSID;
    connectPassPhrase = (WLI_connectPassPhrase_t) WLAN_CONNECT_WPA_PASS;
#endif

#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR == 7)
    if (RETCODE_OK == WlanConnect_WPA(connectSSID, connectPassPhrase, NULL))
    {

        NetworkConfig_GetIpSettings(&myIpSettings);
#else
    if (WLI_SUCCESS == WLI_connectWPA(connectSSID, connectPassPhrase, NULL))
    {
        NCI_getIpSettings(&myIpSettings);
#endif
        WDOG_Feed();
        *IpaddressHex = Basics_htonl(myIpSettings.ipV4);
        (void)Ip_convertAddrToString(IpaddressHex,(char *)&ipAddressMy);
        DEBUG_PRINT("Connected - Ip address of the device: %s",ipAddressMy);
        retVal = XDK_RETCODE_SUCCESS;
    }
    else
    {
        ERR_PRINT("Error occurred connecting %s",WLAN_CONNECT_WPA_SSID);
    }

    return retVal;
}

int WiFiDeinit(void)
{
    DEBUG_PRINT("WiFi disconnect!");
#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR == 7)
    return WlanConnect_Disconnect(NULL);
#else
    return WLI_disconnect(NULL);
#endif
}

void WiFiPrintIP(void)
{
#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR == 7)
    NetworkConfig_IpSettings_T myIpSettings;
#else
    NCI_ipSettings_t myIpSettings;
#endif

    memset(&myIpSettings, 0, sizeof(myIpSettings));

    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();

#if defined(XDKVERSION_H_) && (XDKVERSION_MAJOR == 1) && (XDKVERSION_MINOR == 7)
    NetworkConfig_GetIpSettings(&myIpSettings);
#else
    NCI_getIpSettings(&myIpSettings);
#endif
    *IpaddressHex = Basics_htonl(myIpSettings.ipV4);

    int8_t ipAddressMy[15] = {0};
    (void)Ip_convertAddrToString(IpaddressHex,(char *)&ipAddressMy);

    DEBUG_PRINT("IP address of the device: %s", ipAddressMy);
}
