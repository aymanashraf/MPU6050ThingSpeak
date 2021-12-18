/*
*-----------------------------------------------------------------------------------------------------------------
* Filename         : MPU_CORE.h
* Version          : 0.0.0
* Created          : 2021/04/10
*-----------------------------------------------------------------------------------------------------------------
*/


#ifndef  MPU_CORE_H
#define  MPU_CORE_H

/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                                   # I N C L U D E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

#include "STD_TYPE.h"

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
*                                              # G L O B A L  M A C R O S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/


/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                           G L O B A L  D A T A   T Y P E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
typedef enum
{
    cs2gScaleRange  = 0,
    cs4gScaleRange  = 1,
    cs8gScaleRange  = 2,
    cs16gScaleRange = 3,
    csMaxAccelRange
}MPU_enumAccelScaleType;
typedef enum
{
    cs250ScaleRange  = 0,
    cs500ScaleRange  = 1,
    cs1000ScaleRange = 2,
    cs2000ScaleRange = 3,
    csMaxGyroRange
}MPU_enumGyroScaleType;
/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                           G L O B A L   V A R I A B L E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                                   E X T E R N A L S
*---------------------------------------------------------------------------------------------------------------
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
void          MPU_vInit(void);
StdReturnType MPU_vInitPhase2(void);
StdReturnType MPU_srReadRegister(const INT8U u8RegisterAddr, INT8U* const pu8ReadBuffer, const INT8U u8ReadBytes);
StdReturnType MPU_srWriteRegister(const INT8U u8RegisterAddr, const INT8U u8RegisterValue);
StdReturnType MPU_srSetSleepMode(const BOOLEAN bSleepState);
StdReturnType MPU_srSetAccelerometerScale(const MPU_enumAccelScaleType objenumAccelScaleType);
StdReturnType MPU_srSetGyroscopeScale(const MPU_enumGyroScaleType objenumGyroScaleType);
StdReturnType MPU_srReadTemperature(FP32* const pF32MPUTemperature);
StdReturnType MPU_srReadAccelometerX(FP32* const pF32AcceleometerX);
StdReturnType MPU_srReadAccelometerY(FP32* const pF32AcceleometerY);
StdReturnType MPU_srReadAccelometerZ(FP32* const pF32AcceleometerZ);
StdReturnType MPU_srReadGyroscopeX(FP32* const pF32GyroscopeX);
StdReturnType MPU_srReadGyroscopeY(FP32* const pF32GyroscopeY);
StdReturnType MPU_srReadGyroscopeZ(FP32* const pF32GyroscopeZ);

#ifdef __cplusplus
}
#endif


/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                     MODULE END
*-----------------------------------------------------------------------------------------------------------------*
*/
#endif /* end #ifndef  MPU_CORE_H */


