/*
*-----------------------------------------------------------------------------------------------------------------

*-----------------------------------------------------------------------------------------------------------------
*/


/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                                   # I N C L U D E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
/*
 ==================================================================================================================
 *                                                  Environment Include
 ==================================================================================================================
 */

/*
 ==================================================================================================================
 *                                                  Application Include
 ==================================================================================================================
 */

#include "OS.h"
#include "STD_TYPE.h"
#include "COMPILER_EXT.h"
#include "UTILITY.h"

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                     INCLUDES
*-----------------------------------------------------------------------------------------------------------------*
*/

/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                                   # D E F I N E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/


/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                                # L O C A L  M A C R O S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/


/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                             L O C A L  D A T A   T Y P E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                              L O C A L   V A R I A B L E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/


/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                                L O C A L   T A B L E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/



/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                    L O C A L  F U N C T I O N   P R O T O T Y P E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/


/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                              G L O B A L   V A R I A B L E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/


/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                             G L O B A L   F U N C T I O N S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
/* START FUNCTION DESCRIPTION ******************************************************************************************
OS_srGetOSTaskInformation                                 <OS>

SYNTAX:         StdReturnType OS_srGetOSTaskInformation(OS_strTaskHandlerType    *pobjstrTaskHandlerType,
                                                       strOSTaskInformationType *pobjstrOSTaskInformationType)


DESCRIPTION :   this is an API that will retrieve the OS task information defined by strOSTaskInformationType

PARAMETER1  :   pointer to task handler

PARAMETER2  :   pointer to to OS task information

RETURN VALUE:   srState = E_OK     if the task information is extracted
                        = E_NOT_OK if failed to extract task information
Note        :

END DESCRIPTION *******************************************************************************************************/
StdReturnType OS_srGetOSTaskInformation(OS_strTaskHandlerType    *pstrTaskHandlerType, \
                                         strOSTaskInformationType *pobjstrOSTaskInformationType)
{
    StdReturnType srState;
    Task_Stat     objTaskStateType;

    if((pstrTaskHandlerType != STD_NULL) && (pobjstrOSTaskInformationType!= STD_NULL))
    {
        Task_stat(pstrTaskHandlerType->TaskHandler, &objTaskStateType);
        pobjstrOSTaskInformationType->u16TaskState    = objTaskStateType.mode;
        pobjstrOSTaskInformationType->u16UsedStack     = objTaskStateType.used;
        pobjstrOSTaskInformationType->u16TaskPrioirty  = Task_getPri(pstrTaskHandlerType->TaskHandler);
        pobjstrOSTaskInformationType->s8TaskName       = Task_Handle_name(pstrTaskHandlerType->TaskHandler);
        pobjstrOSTaskInformationType->u16StackSize     = objTaskStateType.stackSize;
        srState                                        = E_OK;
    }
    else
    {
        /* TODO: we should log an error */
        srState = E_NOT_OK;
    }
    return(srState);
}
 /* START FUNCTION DESCRIPTION ******************************************************************************************
 OS_srGetOSQueueInformation                                 <OS>

 SYNTAX:         StdReturnType OS_srGetOSQueueInformation(OS_strQueueHandlerType   *pobjstrQueueHandlerType,
                                                          strOSTaskInformationType *pobjstrOSQueueInformationType)


 DESCRIPTION :   this is an API that will retrieve the OS queue information defined by pobjstrOSQueueInformationType

 PARAMETER1  :   pointer to queue handler

 PARAMETER2  :   pointer to to OS queue information

 RETURN VALUE:   srState = E_OK     if the queue information is extracted
                         = E_NOT_OK if failed to extract queue information
 Note        :

 END DESCRIPTION *******************************************************************************************************/
 StdReturnType OS_srGetOSQueueInformation(OS_strQueueHandlerType    *pobjstrQueueHandlerType, \
                                          strOSQueueInformationType *pobjstrOSQueueInformationType)
 {
     StdReturnType srState = E_OK;

     if((pobjstrQueueHandlerType->MailBoxHandler != STD_NULL) && (pobjstrOSQueueInformationType!= STD_NULL))
     {
         pobjstrOSQueueInformationType->s8QueueName = Mailbox_Handle_name(pobjstrQueueHandlerType->MailBoxHandler);
         pobjstrOSQueueInformationType->u16UsedQueueElement =\
                 Mailbox_getNumPendingMsgs(pobjstrQueueHandlerType->MailBoxHandler);
         pobjstrOSQueueInformationType->u16TotalQueueSize =\
                 pobjstrOSQueueInformationType->u16UsedQueueElement +\
                 Mailbox_getNumFreeMsgs(pobjstrQueueHandlerType->MailBoxHandler);
         pobjstrOSQueueInformationType->u16MsgSize = Mailbox_getMsgSize(pobjstrQueueHandlerType->MailBoxHandler);
     }
     else
     {
         srState = E_NOT_OK;
     }
     return(srState);
 }
 /* START FUNCTION DESCRIPTION ******************************************************************************************
 OS_srGetOSTaskInformation                                 <OS>

 SYNTAX:         void OS_vDelayMS(INT32U u32DelayInMiliSeconds)

 DESCRIPTION :   this is wait for u32DelayInMiliSeconds of milisecond using the OS service delay

 RETURN VALUE:   N/A

 Note        :

 END DESCRIPTION *******************************************************************************************************/
void OS_vDelayMS(INT32U u32DelayInMiliSeconds)
{
    Task_sleep(u32DelayInMiliSeconds);
}


/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                               L O C A L   F U N C T I O N S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
void OS_vTaskCreatehook(Task_Handle Taskhandler)
{

}

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                      MODULE END
*-----------------------------------------------------------------------------------------------------------------*
*/




