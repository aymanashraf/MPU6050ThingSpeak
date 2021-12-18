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
#include "DEM_CORE.h"
#include "DEM_OS.h"
#include "UTILITY.h"
#include "COMPILER_EXT.h"
#include "Verbose.h"
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
/* START FUNCTION DESCRIPTION ******************************************************************************************
DEM_vInit                                           <DEM_CORE>

SYNTAX:         void DEM_vInit(void)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :   Start here

END DESCRIPTION *******************************************************************************************************/
void DEM_vInit(void)
{
    /*Initialize DEM module*/
}
/* START FUNCTION DESCRIPTION ******************************************************************************************
DEM_vLogEvent0                                    <DEM_CORE>

SYNTAX:         void DEM_vLogEvent0(const DEM_enumEventType objenumEventType, const INT8S* const ps8Eventstring)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :   ToDo: What if we have different number of diagnostic parameters

END DESCRIPTION *******************************************************************************************************/
void DEM_vLogEvent0(const DEM_enumEventType objenumEventType, INT8S* const ps8Eventstring)
{
    DEM_strEventInfoType objstrEventInfoType;

    objstrEventInfoType.u32EventTimestamp  = UTI_u32mRTCSecondTimer();
    objstrEventInfoType.objenumEventType   = objenumEventType;
    objstrEventInfoType.ps8EventString     = ps8Eventstring;

    /*Post to DEM mailbox*/
    DEM_srQueueLogEvent(&objstrEventInfoType);
}
/* START FUNCTION DESCRIPTION ******************************************************************************************
DEM_vPrintEventTask                                    <DEM_CORE>

SYNTAX:         void DEM_vPrintEventTask(void)

DESCRIPTION :   used to process log events

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :

END DESCRIPTION *******************************************************************************************************/
void DEM_vPrintEventTask(void)
{
    DEM_strEventInfoType objstrEventInfoType;

    DEM_srDequeueLogEvent(&objstrEventInfoType);
    if(objstrEventInfoType.objenumEventType == csErrorEvent)
    {
        mVerboseNoLine(RED_TEXT);
    }
    mVerboseNoLine("\n%s: %d, %s",\
                   ps8EventString[(INT8U)objstrEventInfoType.objenumEventType],\
                   objstrEventInfoType.u32EventTimestamp,\
                   objstrEventInfoType.ps8EventString);
    if(objstrEventInfoType.objenumEventType == csErrorEvent)
    {
        mVerboseNoLine(WHITE_TEXT);
    }
}
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                      MODULE END
*-----------------------------------------------------------------------------------------------------------------*
*/




