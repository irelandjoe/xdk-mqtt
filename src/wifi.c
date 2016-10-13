#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>

#include "PIp.h"
#include "Serval_Ip.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
#include <FreeRTOS.h>
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

    NCI_ipSettings_t myIpSettings;
    memset(&myIpSettings, (uint32_t) 0, sizeof(myIpSettings));
    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();
    int8_t ipAddressMy[15] = {0};

    WLI_connectSSID_t connectSSID;
    WLI_connectPassPhrase_t connectPassPhrase;

    if(0 == wifiInitDone && WLI_SUCCESS != WLI_init())
    {
        ERR_PRINT("Error occurred initializing WLAN");
        return XDK_RETCODE_FAILURE;
    }

    wifiInitDone = 1;

    DEBUG_PRINT("Connecting to: %s", WLAN_CONNECT_WPA_SSID);

    connectSSID = (WLI_connectSSID_t) WLAN_CONNECT_WPA_SSID;
    connectPassPhrase = (WLI_connectPassPhrase_t) WLAN_CONNECT_WPA_PASS;

    if (WLI_SUCCESS == WLI_connectWPA(connectSSID, connectPassPhrase, NULL))
    {
        WDOG_Feed();
        NCI_getIpSettings(&myIpSettings);
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
    return WLI_disconnect(NULL);
}

void WiFiPrintIP(void)
{
    NCI_ipSettings_t myIpSettings;
    memset(&myIpSettings, 0, sizeof(myIpSettings));

    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();
    NCI_getIpSettings(&myIpSettings);
    *IpaddressHex = Basics_htonl(myIpSettings.ipV4);

    int8_t ipAddressMy[15] = {0};
    (void)Ip_convertAddrToString(IpaddressHex,(char *)&ipAddressMy);

    DEBUG_PRINT("IP address of the device: %s", ipAddressMy);
}

