/*
*-----------------------------------------------------------------------------------------------------------------
* Filename         : ESP8266_CORE.c
* Version          : 0.0.0
* Created          : 2021/12/31
* Author           : Ayman Ashraf
*-----------------------------------------------------------------------------------------------------------------
*/

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                   # I N C L U D E S
*-----------------------------------------------------------------------------------------------------------------
*/
#include <ESP8266_MAIN.h>
#include <Main Library/Environment/COMPILER_EXT.h>
#include <Main Library/Environment/uartstdio.h>
#include "OS.h"
#include "STD_TYPE.h"
#include "PORT_CORE.h"
#include <xdc/runtime/Timestamp.h>
#include "driverlib/uart.h"
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                             # D E F I N E S
*-----------------------------------------------------------------------------------------------------------------*
*/
#define ESP8266_UART_INDEX              0U
#define AT_REQ_FIRMWARE_VERSION         "AT+GMR\r\n"
#define AT_CHECK_CONNECTIVITY           "AT\r\n"
#define AT_REQUEST_AP_INFO              "AT+CWJAP_CUR?\r\n"
#define AT_SET_TX_MODE                  "AT+CIPMODE=%d\r\n"
#define AT_REQ_TX_MODE                  "AT+CIPMODE?\r\n"
#define AT_QUIT_AP_CMD                  "AT+CWQAP\r\n"
#define AT_JOIN_AP_CMD                  "AT+CWJAP_CUR=%s,%s\r\n"
#define AT_SET_WIFI_MODE                "AT+CWMODE_CUR=%d\r\n"
#define AT_REQ_WIFI_MODE                "AT+CWMODE_CUR?\r\n"
#define ESTABLISH_TCP_CONN              "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80,1000\r\n"
#define AT_IPSEND_CMD                   "AT+CIPSEND=%d\r\n"
#define AT_CLOSE_TCP_CONN               "AT+CIPCLOSE\r\n"
#define THINGSPEAK_HTTP_UPDATE          "GET https://api.thingspeak.com/update?api_key=FM9B228YK0IAESB6&field%d=%d\r\n"
#define THINGSPEAK_HTTP_REQ             "GET https://api.thingspeak.com/channels/1605125/fields/%d.json?api_key=J5V9BUXLIOTICQRF&results=%d\r\n"

#define AT_OK_RESPONSE                  "\r\nOK\r\n"
#define AT_SEND_OK_RESPONSE             "\r\nSEND OK\r\n"
#define AT_JOIN_AP_RESPONSE             "\r\nWIFI CONNECTED\r\n"
#define AT_VERSION                      "AT version:"
#define SDK_VERSION                     "SDK version:"
#define COMPILE_TIME                    "compile time:"
#define AP_INFO_START                   "CWJAP_CUR:"
#define AT_TXMODE_RESPONSE              "CIPMODE:"
#define AT_WIFI_MODE_RESPONSE           "CWMODE_CUR:"

#define WIFI_AP_SSID                    "\"WE_AD8FCC\""
#define WIFI_AP_PASS                    "\"AhmedZXC*250\""


#define ESP8266_RESULT_ERROR            0xFFU

#define ERROR_READING_STATUS            -1
#define MINUMUM_RX_LENGTH               1U
#define MAX_READ_SIZE                   1000
#define MAX_SERVER_ENTRIES              10
#define NUMBER_OF_FIELDS                7U
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                   # M A C R O   F U N C T I O N S
*-----------------------------------------------------------------------------------------------------------------*
*/
#define ESP8266_vmSendCommand(u8ATCommandString)\
        UART_write(objESP8266Handler, u8ATCommandString, strlen(u8ATCommandString))
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                              L O C A L   V A R I A B L E S
*-----------------------------------------------------------------------------------------------------------------*
*/
typedef enum
{
    NORMAL_TX_MODE = 0,
    UART_WIFI_PASSTHROUGH = 1
}enumTransmissionModeType;
typedef enum
{
    WIFI_MODE_STATION    = 1,
    WIFI_MODE_AP         = 2,
    WIFI_MODE_STATION_AP = 3
}enumWiFiModeType;
typedef struct
{
    INT8S   s8APSSID[50];
    INT8S   s8APMACAddr[50];
    INT8U   u8APChannel;
    INT32S  s32APRSSI;
    BOOLEAN bAPPCIEnable;
}ESP8266APInfoType;
static ESP8266APInfoType objAPInfoType;
static INT8S  s8ReadBuffer[MAX_READ_SIZE];
static INT32S s32ServerBuffer[MAX_SERVER_ENTRIES];
static UART_Handle   objESP8266Handler;
static INT8S  s8TXModeString[2][20] = {"Normal Mode","Pass through Mode"};
static INT8S  s8WiFiModeStr[3][25]  = {"WiFi Station", "WiFi AP", "WiFi AP and Station"};
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                    L O C A L  F U N C T I O N   P R O T O T Y P E S
*-----------------------------------------------------------------------------------------------------------------*
*/
static void          ESP8266_vWIFIEngine(void);
static void          ESP8266_vEngineTask(INT32U u32Argument1, INT32U u32Argument2);
static StdReturnType ESP8266_srReadResponse(INT8S* ps8ReadBuffer, INT16U* pu16ReadLength);
static StdReturnType ESP8266_srCheckResponse(INT8S* ps8ReadBuffer, INT8S* ps8ExpResponse);
static StdReturnType ESP8266_srExtractFWVersion(INT8S* ps8ReadBuffer);
static StdReturnType ESP8266_srSendAndCheckResponse(INT8S* s8ATSendCommand, INT8S* s8ExpectedRespnse);
static INT8U         ESP8266_srGetSettingValue(INT8S* s8ATSendCommand, INT8S* s8ExpectedRespnse);

static StdReturnType ESP8266_srExtractAPInfo(INT8S* ps8ReadBuffer, ESP8266APInfoType* pobjAccessPointInfo);
static INT8U         ESP8266_u8GetJSONField(INT8S*  s8JSONObjectName,\
                                            INT8S*  s8JSONItemName,\
                                            INT8U   u8NumberOfEntries,\
                                            INT32S* ps32ServerBuffer);
static void          ESP8266_vPrintFoundItems(INT8U u8FoundItems, INT32S* ps32ServerBuffer);

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                             G L O B A L   F U N C T I O N S
*-----------------------------------------------------------------------------------------------------------------*
*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        void ESP8266_vInit(void)

@brief        Used to init ESP8266 UART port at baud rate 115200 bit/s

@param[in]    None

@return       None

@note
-----------------------------------------------------------------------------------------------------------------*/
void ESP8266_vInit(void)
{
     UART_Params objUARTParams;
     Error_Block objstrErrorBlock;
     Task_Params ActionTaskParameter;
     Task_Handle pActionTaskHandler;

     /* create task Action engine Instance*/
     Task_Params_init(&ActionTaskParameter);
     ActionTaskParameter.stackSize                = 1024;
     ActionTaskParameter.priority                 = 3;
     ActionTaskParameter.instance->name           = "ESP8266 Engine Task";
     Error_init(&objstrErrorBlock);
     pActionTaskHandler = Task_create(ESP8266_vEngineTask, &ActionTaskParameter, &objstrErrorBlock);
     (void)pActionTaskHandler;
     UART_Params_init(&objUARTParams);
     objUARTParams.readEcho    = UART_ECHO_OFF;
     objUARTParams.writeDataMode = UART_DATA_BINARY;
     objUARTParams.readReturnMode = UART_RETURN_FULL;
     objUARTParams.readTimeout    = 3000;
     objESP8266Handler = UART_open(ESP8266_UART_INDEX, &objUARTParams);
}
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                      E S P 8 2 6 6     E N G I N E
*-----------------------------------------------------------------------------------------------------------------*
*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        static void ESP8266_vEngineTask(INT32U u32Argument1, INT32U u32Argument2)

@brief        Used as ESP8266 task function that will process ESP8266 operations

@param[in]    u32Argument1: first task argument
@param[in]    u32Argument2: second task argument

@return       E_OK    : operation is done correctly
@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
static void ESP8266_vEngineTask(INT32U u32Argument1, INT32U u32Argument2)
{
    while(1)
    {
        ESP8266_vWIFIEngine();
        OS_vDelayMS(2000U);
    }
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        static void ESP8266_vWIFIEngine(void)

@brief        Used as WIFI engine that will process ESP8266 operations

@param[in]    None

@return       None

@note
-----------------------------------------------------------------------------------------------------------------*/
static void ESP8266_vWIFIEngine(void)
{
    StdReturnType srResponseStatus = E_NOT_OK;
    INT8U         u8ReceivedMode = 0U;
    INT8U         u8FoundItems;

    srResponseStatus = ESP8266_srCheckConnectivity();
    OS_vDelayMS(2000U);
    if(srResponseStatus == E_OK)
    {
        ESP8266_srGetFirmwareVersion();
        ESP8266_srGetAPInfo();
        ESP8266_srSetTransmissionMode(NORMAL_TX_MODE);
        u8ReceivedMode = ESP8266_u8GetTransmissionMode();
        if(u8ReceivedMode != ESP8266_RESULT_ERROR)
        {
            UARTprintf("\nTX Mode: %s", s8TXModeString[u8ReceivedMode]);
        }
        ESP8266_srSetWiFiMode(WIFI_MODE_STATION);
        u8ReceivedMode = ESP8266_u8GetWiFiMode();
        if(u8ReceivedMode != ESP8266_RESULT_ERROR)
        {
            UARTprintf("\nWiFi Mode: %s", s8WiFiModeStr[u8ReceivedMode-1]);
        }
        ESP8266_srQuitAccessPoint();
        ESP8266_srJoinAccessPoint(WIFI_AP_SSID, WIFI_AP_PASS);
        OS_vDelayMS(2000U);
        ESP8266_srUpdateServerItems(5U, 10U);
        ESP8266_srRequestServerItems(5U, 2U, &u8FoundItems, s32ServerBuffer);
        ESP8266_vPrintFoundItems(u8FoundItems, s32ServerBuffer);
    }
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        StdReturnType ESP8266_srSetWiFiMode(INT8U u8ESP8266WiFiModeType)

@brief        Used to Set WIFI mode to be station, access point or both

@param[in]    u8ESP8266WiFiModeType: selected WIFI mode

@return       E_OK    : operation is done correctly

@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
StdReturnType ESP8266_srSetWiFiMode(INT8U u8ESP8266WiFiModeType)
{
    StdReturnType srResponseStatus = E_NOT_OK;
    INT8S         s8TXModeCommand[25];

    if((u8ESP8266WiFiModeType == WIFI_MODE_STATION)||(u8ESP8266WiFiModeType == WIFI_MODE_AP)||\
            (u8ESP8266WiFiModeType == WIFI_MODE_STATION_AP))
    {
        System_snprintf(s8TXModeCommand, sizeof(s8TXModeCommand), AT_SET_WIFI_MODE, u8ESP8266WiFiModeType);
        srResponseStatus = ESP8266_srSendAndCheckResponse(s8TXModeCommand, AT_OK_RESPONSE);
        if(srResponseStatus == E_OK)
        {
            UARTprintf("\nWIFI mode setting: Done");
        }
        else if(srResponseStatus == E_NOT_OK)
        {
            UARTprintf("\nWIFI mode setting: Failed ");
        }
        else
        {
            UARTprintf("\nSerial Port Timeout");
        }
    }
    else
    {
        srResponseStatus = E_NOT_OK;
    }
    return srResponseStatus;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        StdReturnType ESP8266_srSetTransmissionMode(INT8U u8ESP8266TXModeType)

@brief        Used to Set WIFI Transmission mode to be normal or transparent (passthorough)

@param[in]    u8ESP8266TXModeType: selected TX mode

@return       E_OK    : operation is done correctly

@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
StdReturnType ESP8266_srSetTransmissionMode(INT8U u8ESP8266TXModeType)
{
    StdReturnType srResponseStatus = E_NOT_OK;
    INT8S         s8TXModeCommand[15];

    if((u8ESP8266TXModeType == NORMAL_TX_MODE)||(u8ESP8266TXModeType == UART_WIFI_PASSTHROUGH))
    {
        System_snprintf(s8TXModeCommand, sizeof(s8TXModeCommand), AT_SET_TX_MODE, u8ESP8266TXModeType);
        srResponseStatus = ESP8266_srSendAndCheckResponse(s8TXModeCommand, AT_OK_RESPONSE);
        if(srResponseStatus == E_OK)
        {
            UARTprintf("\nTX Mode Setting: Done");
        }
        else if(srResponseStatus == E_NOT_OK)
        {
            UARTprintf("\nTX mode Setting: Failed");
        }
        else
        {
            UARTprintf("\nSerial Port Timeout");
        }
    }
    else
    {
        srResponseStatus = E_NOT_OK;
    }
    return srResponseStatus;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        INT8U ESP8266_u8GetTransmissionMode(void)

@brief        Used to Get WIFI Transmission mode (normal or transparent (pass thorough))

@param[in]    None

@return       WIFI Transmission mode (normal or transparent)

@note
-----------------------------------------------------------------------------------------------------------------*/
INT8U ESP8266_u8GetTransmissionMode(void)
{
    INT8U u8TXModeResult;

    u8TXModeResult = ESP8266_srGetSettingValue(AT_REQ_WIFI_MODE, AT_TXMODE_RESPONSE);
    if((u8TXModeResult != NORMAL_TX_MODE)&&(u8TXModeResult != UART_WIFI_PASSTHROUGH))
    {
        u8TXModeResult = ESP8266_RESULT_ERROR;
    }
    else
    {
        /*No Action*/
    }

    return u8TXModeResult;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        StdReturnType ESP8266_srUpdateServerItems(INT8U u8FieldIndex, INT32S s32FieldValue)

@brief        Used to update specific field in thingspeak cloud with passed value

@param[in]    u8FieldIndex  : index of field to be updated

@param[in]    s32FieldValue : value of field to be set

@return       E_OK    : operation is done correctly

@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
StdReturnType ESP8266_srUpdateServerItems(INT8U u8FieldIndex, INT32S s32FieldValue)
{
    INT8S         s8UpdateStr[120];
    INT8S         s8SendCommand[25];
    StdReturnType srResponseStatus = E_NOT_OK;

    if((u8FieldIndex>=1)&&(u8FieldIndex<=NUMBER_OF_FIELDS))
    {
        srResponseStatus = ESP8266_srSendAndCheckResponse(ESTABLISH_TCP_CONN, AT_OK_RESPONSE);
        if(srResponseStatus == E_OK)
        {
            System_snprintf(s8UpdateStr,sizeof(s8UpdateStr), THINGSPEAK_HTTP_UPDATE, u8FieldIndex, s32FieldValue);
            System_snprintf(s8SendCommand,sizeof(s8SendCommand), AT_IPSEND_CMD, strlen(s8UpdateStr));
            srResponseStatus = ESP8266_srSendAndCheckResponse(s8SendCommand, AT_SEND_OK_RESPONSE);
            if(srResponseStatus == E_OK)
            {
                srResponseStatus = ESP8266_srSendAndCheckResponse(s8UpdateStr, AT_OK_RESPONSE);
            }
        }
    }
    else
    {
        srResponseStatus = E_NOT_OK;
    }
    return srResponseStatus;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        StdReturnType ESP8266_srRequestServerItems(INT8U u8FieldNumber,\
 *                                                       INT8U u8NumberOfEntries,\
 *                                                       INT8U *pu8FoundItems,\
 *                                                       INT32S* ps32ServerBuffer)

@brief        Used to get specific items from thing speak cloud

@param[in]    u8FieldNumber     : index of requested field
@param[in]    u8NumberOfEntries : number of requested entries
@param[in]    pu8FoundItems     : number of received items from cloud
@param[in]    ps32ServerBuffer  : buffer that will contain item values

@return       E_OK    : operation is done correctly
@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
StdReturnType ESP8266_srRequestServerItems(INT8U u8FieldNumber, INT8U u8NumberOfEntries, INT8U *pu8FoundItems, INT32S* ps32ServerBuffer)
{
    INT8S         s8ReqeustStr[120];
    INT8S         s8SendCommand[25];
    StdReturnType srResponseStatus = E_NOT_OK;
    INT8S         s8JSONObjectName[] = "feeds";
    INT8S         s8JSONItemName[10];

    if((u8FieldNumber >= 1)&&(u8FieldNumber <= NUMBER_OF_FIELDS))
    {
        srResponseStatus = ESP8266_srSendAndCheckResponse(ESTABLISH_TCP_CONN, AT_OK_RESPONSE);
        if(srResponseStatus == E_OK)
        {
            System_snprintf(s8ReqeustStr, sizeof(s8ReqeustStr), THINGSPEAK_HTTP_REQ, u8FieldNumber, u8NumberOfEntries);
            System_snprintf(s8SendCommand, sizeof(s8SendCommand), AT_IPSEND_CMD, strlen(s8ReqeustStr));
            srResponseStatus = ESP8266_srSendAndCheckResponse(s8SendCommand, AT_SEND_OK_RESPONSE);
            if(srResponseStatus == E_OK)
            {
                srResponseStatus = ESP8266_srSendAndCheckResponse(s8ReqeustStr, STD_NULL);
                if(srResponseStatus == E_OK)
                {
                    System_snprintf(s8JSONItemName, sizeof(s8JSONItemName), "\"field%d\"", u8FieldNumber);
                    *pu8FoundItems = ESP8266_u8GetJSONField(s8JSONObjectName, s8JSONItemName, u8NumberOfEntries, ps32ServerBuffer);
                }
            }
        }
    }
    else
    {
        srResponseStatus = E_NOT_OK;
    }
    return srResponseStatus;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        INT8U ESP8266_u8GetWiFiMode(void)

@brief        Used to get ESP8266 WIFI Mode (station, access point, both)

@param[in]    None

@return       WIFI Mode (station, access point, both)

@note
-----------------------------------------------------------------------------------------------------------------*/
INT8U ESP8266_u8GetWiFiMode(void)
{
    INT8U u8ModeResult;

    u8ModeResult = ESP8266_srGetSettingValue(AT_REQ_WIFI_MODE, AT_WIFI_MODE_RESPONSE);
    if((u8ModeResult != WIFI_MODE_STATION)&&(u8ModeResult != WIFI_MODE_AP)&&\
            (u8ModeResult != WIFI_MODE_STATION_AP))
    {
        u8ModeResult = ESP8266_RESULT_ERROR;
    }
    else
    {
        /*No Action*/
    }

    return u8ModeResult;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        StdReturnType ESP8266_srCheckConnectivity(void)

@brief        Used to verify ESP8266 is connected

@param[in]    None

@return       E_OK    : WIFI is connected
@return       E_NOT_OK: WIFI is disconnected

@note
-----------------------------------------------------------------------------------------------------------------*/
StdReturnType ESP8266_srCheckConnectivity(void)
{
    StdReturnType srResponseStatus;

    srResponseStatus = ESP8266_srSendAndCheckResponse(AT_CHECK_CONNECTIVITY, AT_OK_RESPONSE);
    if(srResponseStatus == E_OK)
    {
        UARTprintf("\nESP8266 is Connected");
    }
    else if (srResponseStatus == E_NOT_OK)
    {
        UARTprintf("\nESP8266 is Disconnected");
    }
    else
    {
        UARTprintf("\nSerial Port Timeout");
    }
    return srResponseStatus;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        StdReturnType ESP8266_srJoinAccessPoint(INT8S* s8WiFiSSID, INT8S* s8WiFiPass)

@brief        Used to join access point

@param[in]    s8WiFiSSID : Access point ID
@param[in]    s8WiFiPass : Access point Password

@return       E_OK    : operation is done correctly
@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
StdReturnType ESP8266_srJoinAccessPoint(INT8S* s8WiFiSSID, INT8S* s8WiFiPass)
{
    StdReturnType srResponseStatus;
    INT8S         s8JoinCommand[50];

    if((s8WiFiSSID != STD_NULL)&&(s8WiFiPass != STD_NULL))
    {
        System_snprintf(s8JoinCommand, sizeof(s8JoinCommand), AT_JOIN_AP_CMD, s8WiFiSSID, s8WiFiPass);
        srResponseStatus = ESP8266_srSendAndCheckResponse(s8JoinCommand, AT_JOIN_AP_RESPONSE);
        if(srResponseStatus == E_OK)
        {
            UARTprintf("\nESP8266 Join AP: Success");
        }
        else if (srResponseStatus == E_NOT_OK)
        {
            UARTprintf("\nESP8266 Join AP: Fail");
        }
        else
        {
            UARTprintf("\nSerial Port Timeout");
        }
    }
    else
    {

    }
    return srResponseStatus;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        StdReturnType ESP8266_srQuitAccessPoint(void)

@brief        Used to quit access point

@param[in]    None

@return       E_OK    : operation is done correctly
@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
StdReturnType ESP8266_srQuitAccessPoint(void)
{
    StdReturnType srResponseStatus;

    srResponseStatus = ESP8266_srSendAndCheckResponse(AT_QUIT_AP_CMD, AT_OK_RESPONSE);
    if(srResponseStatus == E_OK)
    {
        UARTprintf("\nESP8266 Quit AP");
    }
    else if (srResponseStatus == E_NOT_OK)
    {
        UARTprintf("\nESP8266 Fails Quitting");
    }
    else
    {
        UARTprintf("\nSerial Port Timeout");
    }
    return srResponseStatus;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        StdReturnType ESP8266_srGetFirmwareVersion(void)

@brief        Used to get ESP8266 firmware version (AT version, SDK version, compile time)

@param[in]    None

@return       E_OK    : operation is done correctly
@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
StdReturnType ESP8266_srGetFirmwareVersion(void)
{
    StdReturnType srResponseStatus = E_NOT_OK;
    INT16U        u16ReadLength;

    ESP8266_vmSendCommand(AT_REQ_FIRMWARE_VERSION);
    srResponseStatus = ESP8266_srReadResponse(s8ReadBuffer, &u16ReadLength);
    if(srResponseStatus == E_OK)
    {
        ESP8266_srExtractFWVersion(s8ReadBuffer);
    }
    else
    {
        /*No Action*/
    }

    return srResponseStatus;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        StdReturnType ESP8266_srGetAPInfo(void)

@brief        Used to print and get Access point Info (SSID, MAC, Channel ID, RSSI)

@param[in]    None

@return       E_OK    : operation is done correctly
@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
StdReturnType ESP8266_srGetAPInfo(void)
{
    StdReturnType srResponseStatus = E_NOT_OK;
    INT16U        u16ReadLength;

    ESP8266_vmSendCommand(AT_REQUEST_AP_INFO);
    srResponseStatus = ESP8266_srReadResponse(s8ReadBuffer, &u16ReadLength);
    if(srResponseStatus == E_OK)
    {
        srResponseStatus = ESP8266_srExtractAPInfo(s8ReadBuffer, &objAPInfoType);
        if(srResponseStatus == E_OK)
        {
            UARTprintf("\nAccess Point SSID:%s", objAPInfoType.s8APSSID);
            UARTprintf("\nAccess Point MAC:%s", objAPInfoType.s8APMACAddr);
            UARTprintf("\nAccess Point Channel:%d", objAPInfoType.u8APChannel);
            UARTprintf("\nAccess Point RSSI:%d dB", objAPInfoType.s32APRSSI);
        }
    }
    else
    {
        /*No Action*/
    }

    return srResponseStatus;
}
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                      L O C A L    F U N C T I O N S
*-----------------------------------------------------------------------------------------------------------------*
*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        static StdReturnType ESP8266_srReadResponse(INT8S* ps8ReadBuffer, INT16U* pu16ReadLength)

@brief        Used to read ESP8266 response after sending AT command

@param[in]    ps8ReadBuffer: read buffer that will contain received response from ESP8266

@param[in]    pu16ReadLength: length of received data

@return       E_OK    : operation is done correctly

@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
static StdReturnType ESP8266_srReadResponse(INT8S* ps8ReadBuffer, INT16U* pu16ReadLength)
{
    INT32S        s32ReadBytes;
    StdReturnType srReadStatus = E_NOT_OK;

    s32ReadBytes = UART_read(objESP8266Handler, ps8ReadBuffer, MAX_READ_SIZE);
    if((s32ReadBytes != ERROR_READING_STATUS)&&(s32ReadBytes > MINUMUM_RX_LENGTH))
    {
        srReadStatus = E_OK;
        *pu16ReadLength = s32ReadBytes;
    }
    else
    {
        srReadStatus = E_NOT_OK;
        *pu16ReadLength = 0U;
    }

    return srReadStatus;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        static StdReturnType ESP8266_srCheckResponse(INT8S* ps8ReadBuffer, INT8S* ps8ExpResponse)

@brief        Used to check ESP8266 response and compare it to expected one

@param[in]    ps8ReadBuffer: read buffer that contain received response from ESP8266

@param[in]    ps8ExpResponse: expected response for specific AT command

@return       E_OK    : operation is done correctly

@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
static StdReturnType ESP8266_srCheckResponse(INT8S* ps8ReadBuffer, INT8S* ps8ExpResponse)
{
    INT8S*        ps8ResponseExist = STD_NULL;
    StdReturnType srCheckStatus;

    ps8ResponseExist = strstr(s8ReadBuffer, AT_OK_RESPONSE);
    if(ps8ResponseExist != STD_NULL)
    {
        srCheckStatus = E_OK;
    }
    else
    {
        srCheckStatus = E_NOT_OK;
    }

    return srCheckStatus;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        static StdReturnType ESP8266_srSendAndCheckResponse(INT8S* s8ATSendCommand, INT8S* s8ExpectedRespnse)

@brief        Used to send AT command to ESP8266 and check for its response

@param[in]    s8ATSendCommand: AT command to be sent

@param[in]    ps8ExpResponse: expected response for AT command

@return       E_OK    : operation is done correctly

@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
static StdReturnType ESP8266_srSendAndCheckResponse(INT8S* s8ATSendCommand, INT8S* s8ExpectedRespnse)
{
    StdReturnType srResponseStatus = E_NOT_OK;
    INT16U        u16ReadLength;

    ESP8266_vmSendCommand(s8ATSendCommand);
    srResponseStatus = ESP8266_srReadResponse(s8ReadBuffer, &u16ReadLength);
    if(srResponseStatus == E_OK)
    {
        if(s8ExpectedRespnse != STD_NULL)
        {
            srResponseStatus = ESP8266_srCheckResponse(s8ReadBuffer, s8ExpectedRespnse);
        }
        else
        {
            /*No Action*/
        }
    }
    else
    {
        srResponseStatus = E_INVALID_TIMEOUT;
    }
    return srResponseStatus;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        static INT8U ESP8266_srGetSettingValue(INT8S* s8ATSendCommand, INT8S* s8ExpectedRespnse)

@brief        Used to get setting value like ( Wifi mode, Tx Mode,...)

@param[in]    s8ATSendCommand: AT command to be sent

@param[in]    ps8ExpResponse: expected response for AT command

@return       E_OK    : operation is done correctly

@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
static INT8U ESP8266_srGetSettingValue(INT8S* s8ATSendCommand, INT8S* s8ExpectedRespnse)
{
    INT16U        u16ReadLength;
    INT8S        *ps8ResponseExist;
    StdReturnType srResponseStatus = E_NOT_OK;
    INT8U         u8TXModeResult = ESP8266_RESULT_ERROR;
    INT8S        *ps8LastChar;

    ESP8266_vmSendCommand(s8ATSendCommand);
    srResponseStatus = ESP8266_srReadResponse(s8ReadBuffer, &u16ReadLength);
    if(srResponseStatus == E_OK)
    {
        ps8ResponseExist = strstr(s8ReadBuffer, s8ExpectedRespnse);
        if(ps8ResponseExist != STD_NULL)
        {
            ps8LastChar = strchr(ps8ResponseExist+strlen(s8ExpectedRespnse), '\r');
            *ps8LastChar = 0;
            u8TXModeResult = atoi(ps8ResponseExist+strlen(s8ExpectedRespnse));
            if((u8TXModeResult != NORMAL_TX_MODE)&&(u8TXModeResult != UART_WIFI_PASSTHROUGH))
            {
                u8TXModeResult = ESP8266_RESULT_ERROR;
            }
            else
            {
                /*No Action*/
            }
        }
    }
    else
    {
        u8TXModeResult = ESP8266_RESULT_ERROR;
    }
    return u8TXModeResult;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        static void ESP8266_vPrintFoundItems(INT8U u8FoundItems, INT32S* ps32ServerBuffer)

@brief        Used to print received items from thingspeak cloud

@param[in]    u8FoundItems: number of received items

@param[in]    ps32ServerBuffer: buffer that contain received data

@return       None

@note
-----------------------------------------------------------------------------------------------------------------*/
static void ESP8266_vPrintFoundItems(INT8U u8FoundItems, INT32S* ps32ServerBuffer)
{
    INT8U u8EntryIndex;
    for(u8EntryIndex = 0U; u8EntryIndex < u8FoundItems; u8EntryIndex++)
    {
        UARTprintf("\nItem%d: %d", u8EntryIndex, ps32ServerBuffer[u8EntryIndex]);
    }
}
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                      I N F O   E X T R A C T    F U N C T I O N S
*-----------------------------------------------------------------------------------------------------------------*
*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        static StdReturnType ESP8266_srExtractFWVersion(INT8S* ps8ReadBuffer)

@brief        Used to extract firmware version from received response

@param[in]    ps8ReadBuffer: buffer of received response that contain firmware version

@return       E_OK    : operation is done correctly

@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
static StdReturnType ESP8266_srExtractFWVersion(INT8S* ps8ReadBuffer)
{
    INT8S* ps8ATVersion   = STD_NULL;
    INT8S* ps8SDKVersion  = STD_NULL;
    INT8S* ps8CompileTime = STD_NULL;
    INT8S* ps8LastChar;
    StdReturnType srResponseStatus = E_NOT_OK;

    ps8ATVersion   = strstr(ps8ReadBuffer, AT_VERSION);
    ps8SDKVersion  = strstr(ps8ReadBuffer, SDK_VERSION);
    ps8CompileTime = strstr(ps8ReadBuffer, COMPILE_TIME);
    if(((INT32U)ps8ATVersion & (INT32U)ps8SDKVersion & (INT32U)ps8CompileTime) != 0)
    {
        srResponseStatus = E_OK;
        ps8LastChar    = strchr(ps8ATVersion,'\r');
        *ps8LastChar   = 0;
        UARTprintf("\nAT Version is :%s", ps8ATVersion + strlen(AT_VERSION));
        ps8LastChar    = strchr(ps8SDKVersion,'\r');
        *ps8LastChar   = 0;
        UARTprintf("\nSDK Version is :%s", ps8SDKVersion + strlen(SDK_VERSION));
        ps8LastChar    = strchr(ps8CompileTime,'\r');
        *ps8LastChar   = 0;
        UARTprintf("\nCompile Time is :%s", ps8CompileTime + strlen(COMPILE_TIME));
    }
    else
    {
        srResponseStatus = E_NOT_OK;
    }
    return srResponseStatus;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        static StdReturnType ESP8266_srExtractAPInfo(INT8S* ps8ReadBuffer, ESP8266APInfoType* pobjAccessPointInfo)

@brief        Used to extract access point info like (SSID, MAC, Channel ID, RSSI)

@param[in]    ps8ReadBuffer: received response from ESP8266 that contain AP Info

@param[in]    pobjAccessPointInfo: pointer to object that will contain access point info

@return       E_OK    : operation is done correctly

@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
static StdReturnType ESP8266_srExtractAPInfo(INT8S* ps8ReadBuffer, ESP8266APInfoType* pobjAccessPointInfo)
{
    INT8S* ps8APSSID    = STD_NULL;
    INT8S* ps8APMACAddr = STD_NULL;
    INT8S* ps8APChannel = STD_NULL;
    INT8S* ps8APRSSI    = STD_NULL;
    INT8S* ps8APPCI     = STD_NULL;
    INT8S  s8TempString[50];
    StdReturnType srResponseStatus = E_NOT_OK;

    ps8APSSID    = strstr(ps8ReadBuffer, AP_INFO_START);
    ps8APSSID    = (ps8APSSID!=STD_NULL)?(ps8APSSID+strlen(AP_INFO_START)):STD_NULL;
    ps8APMACAddr = (ps8APSSID!=STD_NULL)?strchr(ps8APSSID, ','):STD_NULL;
    ps8APChannel = (ps8APMACAddr!=STD_NULL)?strchr(ps8APMACAddr+1, ','):STD_NULL;
    ps8APRSSI    = (ps8APChannel!=STD_NULL)?strchr(ps8APChannel+1, ','):STD_NULL;
    ps8APPCI     = (ps8APRSSI!=STD_NULL)?strchr(ps8APRSSI+1, ','):STD_NULL;
    if(((INT32U)ps8APSSID & (INT32U)ps8APMACAddr++ & (INT32U)ps8APChannel++ & (INT32U)ps8APRSSI++ & (INT32U)ps8APPCI++)!= 0)
    {
        srResponseStatus = E_OK;
        System_snprintf(pobjAccessPointInfo->s8APSSID, ps8APMACAddr - ps8APSSID, ps8APSSID);
        System_snprintf(pobjAccessPointInfo->s8APMACAddr, ps8APChannel - ps8APMACAddr, ps8APMACAddr);
        System_snprintf(s8TempString, ps8APRSSI - ps8APChannel, ps8APChannel);
        pobjAccessPointInfo->u8APChannel = atoi(s8TempString);
        System_snprintf(s8TempString, ps8APPCI - ps8APRSSI, ps8APRSSI);
        pobjAccessPointInfo->s32APRSSI = atoi(s8TempString);
    }
    else
    {
        srResponseStatus = E_NOT_OK;
    }
    return srResponseStatus;
}
/*
*-----------------------------------------------------------------------------------------------------------------*
*                              J S O N    P R O C E S S S I N G    F U N C T I O N S
*-----------------------------------------------------------------------------------------------------------------*
*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*!\fn        static INT8U ESP8266_u8GetJSONField(INT8S* s8JSONObjectName, INT8S* s8JSONItemName, INT8U u8NumberOfEntries, INT32S* ps32ServerBuffer)

@brief        Used to extract specific field from JSON object received from thing speak cloud

@param[in]    s8JSONObjectName: name of JSON object under which field existas

@param[in]    s8JSONItemName: name of JSON item

@param[in]    u8NumberOfEntries: number of received entries

@param[in]    ps32ServerBuffer: buffer that contain data

@return       E_OK    : operation is done correctly

@return       E_NOT_OK: something goes wrong

@note
-----------------------------------------------------------------------------------------------------------------*/
static INT8U ESP8266_u8GetJSONField(INT8S* s8JSONObjectName, INT8S* s8JSONItemName, INT8U u8NumberOfEntries, INT32S* ps32ServerBuffer)
{
    INT8S         *ps8StringExist;
    INT8S         *ps8ValueStart;
    INT8S         *ps8ValueEnd;
    INT8S         s8StringTemp[10];
    INT8U         u8FoundItems = 0U;

    /*i.e "field5":"10"*/
    /*Point to JSON Object Name start*/
    ps8StringExist = strstr(s8ReadBuffer, s8JSONObjectName);
    ps8StringExist = (ps8StringExist!=STD_NULL)?(ps8StringExist + strlen(s8JSONObjectName)):STD_NULL;
    while((ps8StringExist != STD_NULL)&&(u8FoundItems < u8NumberOfEntries))
    {
        /*Point to JSON Item Name start*/
        ps8StringExist = (ps8StringExist!=STD_NULL)?(strstr(ps8StringExist, s8JSONItemName)):STD_NULL;
        ps8StringExist = (ps8StringExist!=STD_NULL)?(ps8StringExist + strlen(s8JSONItemName)):STD_NULL;
        ps8ValueStart  = (ps8StringExist!=STD_NULL)?(strchr(ps8StringExist, '"')+1U):STD_NULL;
        ps8ValueEnd    = (ps8ValueStart!=STD_NULL)?(strchr(ps8ValueStart, '"')):STD_NULL;
        if((ps8ValueStart!= STD_NULL) && (ps8ValueEnd!= STD_NULL))
        {
            strncpy(s8StringTemp, ps8ValueStart, ps8ValueEnd - ps8ValueStart);
            s8StringTemp[ps8ValueEnd - ps8ValueStart] = 0U;
            *(ps32ServerBuffer + (INT32U)u8FoundItems++) = atoi(s8StringTemp);
        }
        else
        {
            break;
        }
    }
    return u8FoundItems;
}
