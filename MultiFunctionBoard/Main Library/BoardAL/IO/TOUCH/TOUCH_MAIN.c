/*
*-----------------------------------------------------------------------------------------------------------------
* Filename         : TOUCH_CORE.c
* Version          : 0.0.0
* Created          : 2021/31/12
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
#include <Main Library/Environment/uartstdio.h>
#include <TOUCH_MAIN.h>
#include "DEM_CORE.h"
#include "IOHWAB_DIGITAL.h"
#include "OS.h"

/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                                   # D E F I N E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                    L O C A L  F U N C T I O N   P R O T O T Y P E S
*-----------------------------------------------------------------------------------------------------------------*
*/
static void TOUCH_vMonitorTask(INT32U u32Argument1, INT32U u32Argument2);
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                             G L O B A L   F U N C T I O N S
*-----------------------------------------------------------------------------------------------------------------*
*/
/*-----------------------------------------------------------------------------------------------------------------*/

/*!fn        void TOUCH_vInit(void)

@brief   None

@param[in]  :   None

@return   None

@note        :   None

-----------------------------------------------------------------------------------------------------------------------------*/
void TOUCH_vInit(void)
{
    Error_Block     objstrErrorBlock;
    Task_Params     ActionTaskParameter;
    Task_Handle     pActionTaskHandler;

    /* create task Action engine Instance*/
    Task_Params_init(&ActionTaskParameter);
    ActionTaskParameter.stackSize                = 512;
    ActionTaskParameter.priority                 = 3;
    ActionTaskParameter.instance->name           = "TOUCH Monitor Task";
    Error_init(&objstrErrorBlock);
    pActionTaskHandler = Task_create(TOUCH_vMonitorTask, &ActionTaskParameter, &objstrErrorBlock);
    (void)pActionTaskHandler;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/*!fn        static void TOUCH_vMonitorTask(INT32U u32Argument1, INT32U u32Argument2)

@brief   None

@param[in]  :   None

@return   None

@note        :

-----------------------------------------------------------------------------------------------------------------------------*/
static void TOUCH_vMonitorTask(INT32U u32Argument1, INT32U u32Argument2)
{

    OS_vDelayMS(5000U);
    while(1)
    {
        TOUCH_vMonitor();
        OS_vDelayMS(12000U);
    }
}
/*-----------------------------------------------------------------------------------------------------------------*/

/*!fn        void TOUCH_vMonitor(void)

@brief   None

@param[in]  :   None

@return   None

@note        :   the process function shall be called inside infinite loop

-----------------------------------------------------------------------------------------------------------------------------*/
void TOUCH_vMonitor(void)
{
    INT8U u8TouchValue = STD_LOW;

    u8TouchValue = IOHWAB_u8GetDigitalValue(csTouchInputChannel);

    if(u8TouchValue == STD_LOW)
    {
        DEM_vLogActivity0("Touch Signal is Low");
    }
    else
    {
        DEM_vLogActivity0("Touch Signal is High");
    }
}
