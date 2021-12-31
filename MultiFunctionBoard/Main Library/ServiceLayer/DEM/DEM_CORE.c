/*
*-----------------------------------------------------------------------------------------------------------------
* Version          : 0.0.0
* Created          : 2021/04/10
* Author           : Ayman Ashraf
*-----------------------------------------------------------------------------------------------------------------
*/



/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                   # I N C L U D E S
*-----------------------------------------------------------------------------------------------------------------*
*/
#include <Main Library/Environment/COMPILER_EXT.h>
#include <Main Library/Environment/uartstdio.h>
#include <Main Library/Environment/UTILITY.h>
#include "DEM_CORE.h"
#include "DEM_OS.h"
#include "OS.h"

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                             L O C A L  D A T A   T Y P E S
*-----------------------------------------------------------------------------------------------------------------*
*/

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                              L O C A L   V A R I A B L E S
*-----------------------------------------------------------------------------------------------------------------*
*/
static INT8S* ps8EventString[] = {"Activity", "Error"};
/*
 *-----------------------------------------------------------------------------------------------------------------*
*-----------------------------------------------------------------------------------------------------------------*
*                                    L O C A L  F U N C T I O N   P R O T O T Y P E S
*-----------------------------------------------------------------------------------------------------------------*
*/
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                              G L O B A L   V A R I A B L E S
*-----------------------------------------------------------------------------------------------------------------*
*/

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                             G L O B A L   F U N C T I O N S
*-----------------------------------------------------------------------------------------------------------------*
*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*! fn        void DEM_vInit(void)

@brief   None

@param[in]  :   None

@return   None

@note        :   None

-----------------------------------------------------------------------------------------------------------------------------*/
void DEM_vInit(void)
{
    /*Initialize DEM module*/
}
/*-----------------------------------------------------------------------------------------------------------------------*/

/*! fn        void DEM_vLogEvent0(const DEM_enumEventType objenumEventType, const INT8S* const ps8Eventstring)

@brief   None

@param[in]  :   None

@return   None

@note        :   ToDo: What if we have different number of diagnostic parameters

-----------------------------------------------------------------------------------------------------------------------------*/
void DEM_vLogEvent0(const DEM_enumEventType objenumEventType, INT8S* const ps8Eventstring)
{
    DEM_strEventInfoType objstrEventInfoType;

    objstrEventInfoType.u32EventTimestamp  = UTI_u32mRTCSecondTimer();
    objstrEventInfoType.objenumEventType   = objenumEventType;
    objstrEventInfoType.ps8EventString     = ps8Eventstring;

    /*Post to DEM mailbox*/
    DEM_srQueueLogEvent(&objstrEventInfoType);
}
/*-----------------------------------------------------------------------------------------------------------------------*/
/*! fn        void DEM_vPrintEventTask(void)

@brief   used to process log events

@param[in]  :   None

@return   None

@note        :

-----------------------------------------------------------------------------------------------------------------------------*/
void DEM_vPrintEventTask(void)
{
    DEM_strEventInfoType objstrEventInfoType;

    DEM_srDequeueLogEvent(&objstrEventInfoType);
    if(objstrEventInfoType.objenumEventType == csErrorEvent)
    {
        UARTprintf(RED_TEXT);
    }
    UARTprintf("\n%s: %d, %s",\
                   ps8EventString[(INT8U)objstrEventInfoType.objenumEventType],\
                   objstrEventInfoType.u32EventTimestamp,\
                   objstrEventInfoType.ps8EventString);
    if(objstrEventInfoType.objenumEventType == csErrorEvent)
    {
        UARTprintf(WHITE_TEXT);
    }
}
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                      MODULE END
*-----------------------------------------------------------------------------------------------------------------*
*/




