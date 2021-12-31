/*
*-----------------------------------------------------------------------------------------------------------------
* Filename         : DEM_OS.c
* Version          : 0.0.0
* Modified         : 2021/31/12
* Author           : Ayman Ashraf
*-----------------------------------------------------------------------------------------------------------------
*
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
*                                                   # D E F I N E S
*-----------------------------------------------------------------------------------------------------------------*
*/
#define MAX_SERIAL_QUEUED_EVENTS        20U


/*
*-----------------------------------------------------------------------------------------------------------------*
*                                             L O C A L  D A T A   T Y P E S
*-----------------------------------------------------------------------------------------------------------------*
*/
static Mailbox_Handle objLogMailboxHandler;
/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                    L O C A L  F U N C T I O N   P R O T O T Y P E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
static void DEM_vProcessTask(INT32U u32Argument1, INT32U u32Argument2);
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
/*! fn        void DEM_vInitOS(void)

@brief   None

@param[in]  :   None

@return   None

@note        :   None

-----------------------------------------------------------------------------------------------------------------------------*/
void DEM_vInitOS(void)
{
    Task_Params     PrintTaskParameter;
    Task_Handle     pPrintTaskHandler;
    Mailbox_Params  objLogMailboxParameter;

    /* create task Print engine Instance*/
    Task_Params_init(&PrintTaskParameter);
    PrintTaskParameter.stackSize                = 512;
    PrintTaskParameter.priority                 = 2;
    PrintTaskParameter.instance->name           = "DEM Process Task";
    pPrintTaskHandler = Task_create(DEM_vProcessTask, &PrintTaskParameter, STD_NULL);
    (void)pPrintTaskHandler;
    /* create a Mailbox Instance */
    Mailbox_Params_init(&objLogMailboxParameter);
    objLogMailboxParameter.instance->name = "Serial log Q";
    objLogMailboxHandler = Mailbox_create(sizeof(DEM_strEventInfoType),\
                                          MAX_SERIAL_QUEUED_EVENTS,\
                                          &objLogMailboxParameter,\
                                          STD_NULL);
}
/*-----------------------------------------------------------------------------------------------------------------------*/
/*! fn        StdReturnType DEM_srQueueSerialLogEvent(strEventInfoType const *pstrEventInfoType);

@brief   This function is used to Q log event into Serial log event mailbox

@param[in]  :  Pointer to the activity which would be queued

@return  E_OK             : if the queuing  process success
               E_NOT_OK         : if the queuing  process  fail

@note        :

-----------------------------------------------------------------------------------------------------------------------------*/
StdReturnType DEM_srQueueLogEvent(DEM_strEventInfoType const *pstrEventInfoType)
{
    BOOLEAN       bPostStatus;
    StdReturnType srQueueStatus = E_OK;

    /*enqueue message*/
    bPostStatus = Mailbox_post(objLogMailboxHandler, (xdc_Ptr)pstrEventInfoType, BIOS_NO_WAIT);
    if(bPostStatus == STD_FALSE)
    {
        /*
            UARTprintf(RED_TEXT);
            UARTprintf("\ncan't add to serial log Q");
            UARTprintf(WHITE_TEXT);
            srQueueStatus = E_NOT_OK;
        */
    }
    return srQueueStatus;
}
/*-----------------------------------------------------------------------------------------------------------------------*/
/*! fn        StdReturnType DEM_srDequeueLogEvent(strEventInfoType **pstrEventInfoType)

@brief   pend the log mailbox for event

@param[in]  :   address of pointer to the log event

@return   E_OK             : if the dequeue process is success
                E_NOT_OK         : if something wrong occurs

@note        :

-----------------------------------------------------------------------------------------------------------------------------*/
StdReturnType DEM_srDequeueLogEvent(DEM_strEventInfoType *pstrEventInfoType)
{
    StdReturnType     srQueueStatus = E_OK;
    BOOLEAN           bPendingStatus;

    bPendingStatus = Mailbox_pend(objLogMailboxHandler, pstrEventInfoType, OS_WAIT_FOREVER);
    if(bPendingStatus == STD_FALSE)                               /* no error encountered in the mail box */
    {
        pstrEventInfoType = STD_NULL;
        srQueueStatus = E_NOT_OK;
    }

    return srQueueStatus;
}
/*-----------------------------------------------------------------------------------------------------------------------*/

/*! fn        static void DEM_vMonitorTask(INT32U u32Argument1, INT32U u32Argument2);

@brief   None

@param[in]  :   None

@return   None

@note        :

-----------------------------------------------------------------------------------------------------------------------------*/
static void DEM_vProcessTask(INT32U u32Argument1, INT32U u32Argument2)
{
    (void)u32Argument1;
    (void)u32Argument2;
    while(1)
    {
        DEM_vPrintEventTask();
    }
}
