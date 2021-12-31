#ifndef  ESP8266_CORE_H
#define  ESP8266_CORE_H

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                   # I N C L U D E S
*-----------------------------------------------------------------------------------------------------------------
*/
#include <Main Library/Environment/COMPILER_EXT.h>
#include "STD_TYPE.h"

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                   G L O B A L   F U N C T I O N S   P R O T O T Y P E S
*-----------------------------------------------------------------------------------------------------------------*
*/
/* Put the functions here */
void          ESP8266_vInit(void);
StdReturnType ESP8266_srGetFirmwareVersion(void);
StdReturnType ESP8266_srCheckConnectivity(void);
StdReturnType ESP8266_srGetAPInfo(void);
INT8U         ESP8266_u8GetWiFiMode(void);
StdReturnType ESP8266_srSetWiFiMode(INT8U u8ESP8266WiFiModeType);
StdReturnType ESP8266_srSetTransmissionMode(INT8U u8ESP8266TXModeType);
INT8U         ESP8266_u8GetTransmissionMode(void);
StdReturnType ESP8266_srJoinAccessPoint(INT8S* s8WiFiSSID, INT8S* s8WiFiPass);
StdReturnType ESP8266_srQuitAccessPoint(void);
StdReturnType ESP8266_srUpdateServerItems(INT8U u8FieldIndex , INT32S s32FieldValue);
StdReturnType ESP8266_srRequestServerItems(INT8U  u8FieldNumber,\
                                           INT8U  u8NumberOfEntries,\
                                           INT8U  *pu8FoundItems,\
                                           INT32S *ps32ServerBuffer);

#endif /* end #ifndef  ESP8266_CORE_H */



