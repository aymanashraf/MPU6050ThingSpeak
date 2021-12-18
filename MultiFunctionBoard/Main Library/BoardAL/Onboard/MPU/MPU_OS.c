/*
*-----------------------------------------------------------------------------------------------------------------
* Filename         : MPU_OS.c
* Version          : 0.0.0
* Created          : 2021/04/10
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
#include <Main Library/BoardAL/Onboard/MPU/MPU_CORE.h>
#include <Main Library/BoardAL/Onboard/MPU/MPU_OS.h>
#include "DEM_CORE.h"
#include "OS.h"
#include "COMPILER_EXT.h"
#include "UTILITY.h"
#include "Verbose.h"
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                     INCLUDES
*******************************************************************************************************************/



/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                                   # D E F I N E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
#define WHO_AM_I_R                      0x75U
#define SELF_TEST_X                     0x0DU
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
typedef struct
{
    FP32   XAccel;
    FP32   YAccel;
    FP32   ZAccel;
}MPU_strAcclerationType;
typedef struct
{
    FP32   XGyro;
    FP32   YGyro;
    FP32   ZGyro;
}MPU_strGyroscopeType;

/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                              L O C A L   V A R I A B L E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
//static INT8S *ps8LogDirectoryName = "/Log";
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
static void MPU_vMonitorTask(INT32U u32Argument1, INT32U u32Argument2);
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
MPU_vInitOS                                         <MPU_CORE>

SYNTAX:         void MPU_vInitOS(void)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :   Start here

END DESCRIPTION *******************************************************************************************************/
void MPU_vInitOS(void)
{
    Error_Block     objstrErrorBlock;
    Task_Params     ActionTaskParameter;
    Task_Handle     pActionTaskHandler;
    BOOLEAN         bErrorStatus;

     /* create task Action engine Instance*/
     Task_Params_init(&ActionTaskParameter);
     ActionTaskParameter.stackSize                = 512;
     ActionTaskParameter.priority                 = 3;
     ActionTaskParameter.instance->name           = "MPU Monitor Task";
     Error_init(&objstrErrorBlock);
     pActionTaskHandler = Task_create(MPU_vMonitorTask, &ActionTaskParameter, &objstrErrorBlock);
     bErrorStatus = Error_check(&objstrErrorBlock);
     if((pActionTaskHandler == STD_NULL) ||  (bErrorStatus == STD_TRUE))
     {
         /*Error can't create task*/
     }
     else
     {
         /*Register Task Handler*/
     }
     MPU_vInit();
}
/* START FUNCTION DESCRIPTION ******************************************************************************************
MPU_vProcess                               <MPU_CORE>

SYNTAX:         static void MPU_vMonitorTask(INT32U u32Argument1, INT32U u32Argument2);

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :

END DESCRIPTION *******************************************************************************************************/
static void MPU_vMonitorTask(INT32U u32Argument1, INT32U u32Argument2)
{
    INT8U  u8RegisterValue;
    static INT8U u8TestValue = 0U;
    FP32   F32MPUTemperature;
    MPU_strAcclerationType objstrAcclerationType = {0};
    MPU_strGyroscopeType   objstrGyroscopeType   = {0};
    StdReturnType          srAccelReadStatus     = E_OK;
    StdReturnType          srGyroReadStatus      = E_OK;
    INT8S                  s8TempString[10];
    //FRESULT                objFRESULT;

    MPU_vInitPhase2();
    while(1)
    {

        OS_vDelayMS(1000U);
        if(MPU_srReadRegister(WHO_AM_I_R, &u8RegisterValue, 1U) == E_OK)
        {
            mVerboseNoLine("\nWho AM I: 0x%x", u8RegisterValue);
        }
        else
        {
            /*No Action*/
        }
        if(MPU_srWriteRegister(SELF_TEST_X, u8TestValue++) == E_OK)
        {
            if(MPU_srReadRegister(SELF_TEST_X, &u8RegisterValue, 1U) == E_OK)
            {
                mVerboseNoLine("\nMPU Test Value is %d", u8RegisterValue);
            }
            else
            {
                /*No Action*/
            }
        }
        else
        {
            /*No Action*/
        }
        if(MPU_srReadTemperature(&F32MPUTemperature) == E_OK)
        {
            mVerboseNoLine("\nMPU Temperature is: %d degree celsius", (INT16U)F32MPUTemperature);
        }
        else
        {
            /*No Action*/
        }
        srAccelReadStatus |= MPU_srReadAccelometerX(&objstrAcclerationType.XAccel);
        srAccelReadStatus |= MPU_srReadAccelometerY(&objstrAcclerationType.YAccel);
        srAccelReadStatus |= MPU_srReadAccelometerZ(&objstrAcclerationType.ZAccel);
        if(srAccelReadStatus == E_OK)
        {
            mVerboseNoLine("\nAcceleation Components are:");
            UTIL_ftoa(objstrAcclerationType.XAccel, s8TempString ,2);
            mVerboseNoLine("\nAx: %s", s8TempString);
            UTIL_ftoa(objstrAcclerationType.YAccel, s8TempString ,2);
            mVerboseNoLine("\nAy: %s", s8TempString);
            UTIL_ftoa(objstrAcclerationType.ZAccel, s8TempString ,2);
            mVerboseNoLine("\nAz: %s", s8TempString);
        }
        else
        {
            /*No Action*/
        }
        srGyroReadStatus |= MPU_srReadGyroscopeX(&objstrGyroscopeType.XGyro);
        srGyroReadStatus |= MPU_srReadGyroscopeY(&objstrGyroscopeType.YGyro);
        srGyroReadStatus |= MPU_srReadGyroscopeZ(&objstrGyroscopeType.ZGyro);
        if(srGyroReadStatus == E_OK)
        {
            mVerboseNoLine("\n\nGyroscope Components are:");
            UTIL_ftoa(objstrGyroscopeType.XGyro, s8TempString ,2);
            mVerboseNoLine("\nGx: %s", s8TempString);
            UTIL_ftoa(objstrGyroscopeType.XGyro, s8TempString ,2);
            mVerboseNoLine("\nGy: %s", s8TempString);
            UTIL_ftoa(objstrGyroscopeType.ZGyro, s8TempString ,2);
            mVerboseNoLine("\nGz: %s", s8TempString);
        }
        else
        {
            /*No Action*/
        }
    }

}
/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                               L O C A L   F U N C T I O N S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                      MODULE END
*-----------------------------------------------------------------------------------------------------------------*
*/
