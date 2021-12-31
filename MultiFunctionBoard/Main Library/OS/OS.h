/*
*-----------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------
*/

#ifndef  __OS_H
#define  __OS_H

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                     INCLUDES
*-----------------------------------------------------------------------------------------------------------------*
*/

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/mailbox.h>


#include "STD_TYPE.h"
/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                                   # D E F I N E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

#define TASK_RUNNING                   ti_sysbios_knl_Task_Mode_RUNNING
#define TASK_READY                     ti_sysbios_knl_Task_Mode_READY
#define TASK_BLOCKED                   ti_sysbios_knl_Task_Mode_BLOCKED
#define TASK_TERMINATED                ti_sysbios_knl_Task_Mode_TERMINATED
#define TASK_INACTIVE                  ti_sysbios_knl_Task_Mode_INACTIVE


/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                              # G L O B A L  M A C R O S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

#define OS_WAIT_FOREVER                 BIOS_WAIT_FOREVER
#define OS_NO_WAIT                      BIOS_NO_WAIT
/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                           G L O B A L  D A T A   T Y P E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

typedef struct
{
    Task_Handle TaskHandler;
}OS_strTaskHandlerType;
/* we will use the mailbox as a queue, as if the SYS/BIOS give a mailbox name
 * to a queue functionality
 */
typedef struct
{
    Mailbox_Handle MailBoxHandler;
}OS_strQueueHandlerType;
typedef struct
{
    INT16U u16TotalQueueSize;
    INT16U u16UsedQueueElement;
    INT8S *s8QueueName;
    INT16U u16MsgSize;
}strOSQueueInformationType;

typedef struct
{
    INT16U u16StackSize;
    INT16U u16UsedStack;
    INT16U u16TaskState;
    INT16U u16TaskPrioirty;
    INT8S *s8TaskName;
}strOSTaskInformationType;
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                   E X T E R N A L S
*-----------------------------------------------------------------------------------------------------------------*
*/


/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                    G L O B A L  F U N C T I O N   P R O T O T Y P E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

#ifdef __cplusplus
extern "C" {
#endif

/* Put the functions here */
void          OS_vDelayMS(INT32U u32DelayInMiliSeconds);
#ifdef __cplusplus
}
#endif


/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                     MODULE END
*-----------------------------------------------------------------------------------------------------------------*
*/
#endif



