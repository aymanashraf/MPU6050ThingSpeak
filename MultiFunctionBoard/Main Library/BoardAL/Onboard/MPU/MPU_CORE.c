/*
*-------------------------------------------------------------------------------------------------------------
* Filename         : MPU_CORE.c
* Version          : 0.0.0
* Created          : 2021/04/10
*-------------------------------------------------------------------------------------------------------------
*
*-------------------------------------------------------------------------------------------------------------
*/




/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                   # I N C L U D E S
*-----------------------------------------------------------------------------------------------------------------*
*/
#include "MPU_CORE.h"
#include "COMPILER_EXT.h"
#include "Verbose.h"
#include "OS.h"

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                   # D E F I N E S
*-----------------------------------------------------------------------------------------------------------------*
*/
#define MPU6050_I2C_INDEX           0U
#define TEMP_OUT_R                  0x41U
#define ACCEL_XOUT_R                0x3BU
#define ACCEL_YOUT_R                0x3DU
#define ACCEL_ZOUT_R                0x3FU
#define GYRO_XOUT_R                 0x43U
#define GYRO_YOUT_R                 0x45U
#define GYRO_ZOUT_R                 0x47U
#define PWR_MGMT_1_R                0x6BU
#define ACCEL_CONFIG_R              0x1CU
#define GYRO_CONFIG_R               0x1BU
#define ACCEL_2G_SEL_M              (0x00<<3U)
#define ACCEL_4G_SEL_M              (0x01<<3U)
#define ACCEL_8G_SEL_M              (0x02<<3U)
#define ACCEL_16G_SEL_M             (0x03<<3U)
#define ACCEL_SCALE_SEL_M           (0x03<<3U)
#define GYRO_250_SEL_M              (0x00<<3U)
#define GYRO_500_SEL_M              (0x01<<3U)
#define GYRO_1000_SEL_M             (0x02<<3U)
#define GYRO_2000_SEL_M             (0x03<<3U)
#define GYRO_SCALE_SEL_M            (0x03<<3U)
#define PWR_SLEEP_MODE_B            (1<<6U)
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                # L O C A L  M A C R O S
*-----------------------------------------------------------------------------------------------------------------*
*/
#define SWAP_VALUE16(u16RegisterValue)    ((u16RegisterValue>>8)|((u16RegisterValue&0xFF)<<8))
#define SET_BIT(REG, MASK, VAL)            ((REG&(~MASK))|VAL)
/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                             L O C A L  D A T A   T Y P E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
typedef struct
{
    MPU_enumAccelScaleType objenumAccelScaleType;
    FP32                   FP32AccelSensitivity;
    INT8S                  s8ConfigName[20];
}MPU_strAccelConfigType;
typedef struct
{
    MPU_enumGyroScaleType objenumGyroScaleType;
    FP32                  FP32GyroSensitivity;
    INT8S                 s8ConfigName[20];
}MPU_strGyroConfigType;
typedef struct
{
    MPU_strAccelConfigType objstrAccelConfigType;
    MPU_strGyroConfigType  objstrGyroConfigType;
}MPU_strMPUConfigType;
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                              L O C A L   V A R I A B L E S
*-----------------------------------------------------------------------------------------------------------------*
*/
static I2C_Handle MPU6050_I2CHandle;
const MPU_strGyroConfigType  objstrGyroConfig[4]  = {{.objenumGyroScaleType = cs250ScaleRange, .FP32GyroSensitivity = 131.0, .s8ConfigName = "250 Scale Range"},\
                                                     {.objenumGyroScaleType = cs500ScaleRange, .FP32GyroSensitivity = 65.5, .s8ConfigName = "500 Scale Range"},\
                                                     {.objenumGyroScaleType = cs1000ScaleRange, .FP32GyroSensitivity = 32.8, .s8ConfigName = "1000 Scale Range"},\
                                                     {.objenumGyroScaleType = cs2000ScaleRange, .FP32GyroSensitivity = 16.4, .s8ConfigName = "2000 Scale Range"}};

const MPU_strAccelConfigType objstrAccelConfig[4] = {{.objenumAccelScaleType = cs2gScaleRange, .FP32AccelSensitivity = (16384.0/9.81), .s8ConfigName = "2g Scale Range"},\
                                                     {.objenumAccelScaleType = cs4gScaleRange, .FP32AccelSensitivity = (8192.0/9.81), .s8ConfigName = "4g Scale Range"},\
                                                     {.objenumAccelScaleType = cs8gScaleRange, .FP32AccelSensitivity = (4096.0/9.81), .s8ConfigName = "8g Scale Range"},\
                                                     {.objenumAccelScaleType = cs16gScaleRange, .FP32AccelSensitivity = (2048.0/9.81), .s8ConfigName = "16g Scale Range"}};


static MPU_strMPUConfigType objstrMPUConfigType;

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                   L O C A L  F U N C T I O N   P R O T O T Y P E S
*-----------------------------------------------------------------------------------------------------------------*
*/

/*
*---------------------------------------------------------------------------------------------------------------
*                                              G L O B A L   V A R I A B L E S
*---------------------------------------------------------------------------------------------------------------
*/

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                             G L O B A L   F U N C T I O N S
*-----------------------------------------------------------------------------------------------------------------*
*/
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_vInit                                           <MPU_CORE>

SYNTAX:         void MPU_vInit(void)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
void MPU_vInit(void)
{
    I2C_Params objI2C_Params;

    I2C_Params_init(&objI2C_Params);
    objI2C_Params.bitRate      = I2C_400kHz;
    objI2C_Params.transferMode = I2C_MODE_BLOCKING;
    MPU6050_I2CHandle = I2C_open(MPU6050_I2C_INDEX, &objI2C_Params);
    objstrMPUConfigType.objstrAccelConfigType = objstrAccelConfig[cs16gScaleRange];
    objstrMPUConfigType.objstrGyroConfigType  = objstrGyroConfig[cs2000ScaleRange];
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_vInitPhase2                                     <MPU_CORE>

SYNTAX:         StdReturnType MPU_vInitPhase2(void)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_vInitPhase2(void)
{
    StdReturnType srInitStatus = E_OK;

    /*Disable Sleep Mode to enable Register Writing*/
    srInitStatus = MPU_srSetSleepMode(STD_FALSE);
    if(srInitStatus == E_OK)
    {
        MPU_srSetAccelerometerScale(objstrMPUConfigType.objstrAccelConfigType.objenumAccelScaleType);
        MPU_srSetGyroscopeScale(objstrMPUConfigType.objstrGyroConfigType.objenumGyroScaleType);
    }
    mVerboseNoLine("\nMPU Accelerometer Full scale range is %s g", objstrMPUConfigType.objstrAccelConfigType.s8ConfigName);
    mVerboseNoLine("\nMPU Gyroscope Full scale range is %s °/sec", objstrMPUConfigType.objstrGyroConfigType.s8ConfigName);
    return srInitStatus;
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_srReadRegister                                    <MPU_CORE>

SYNTAX:         StdReturnType MPU_srReadRegister(const INT8U u8RegisterAddr, INT8U* const pu8ReadBuffer, const INT8U u8ReadBytes)

DESCRIPTION :   used to read MPU6050 Register

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_srReadRegister(const INT8U u8RegisterAddr, INT8U* const pu8ReadBuffer, const INT8U u8ReadBytes)
{
    I2C_Transaction MPU6050_I2CTransaction;
    INT8U           u8RegisterAddress[] = {u8RegisterAddr};
    StdReturnType   srReadStatus = E_OK;

    MPU6050_I2CTransaction.slaveAddress = 0x68;
    MPU6050_I2CTransaction.writeCount   = 1U;
    MPU6050_I2CTransaction.writeBuf     = u8RegisterAddress;
    MPU6050_I2CTransaction.readCount    = u8ReadBytes;
    MPU6050_I2CTransaction.readBuf      = pu8ReadBuffer;
    if(I2C_transfer(MPU6050_I2CHandle, &MPU6050_I2CTransaction))
    {
        srReadStatus = E_OK;
    }
    else
    {
        srReadStatus = E_NOT_OK;
    }

    return srReadStatus;
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_srWriteRegister                                 <MPU_CORE>

SYNTAX:         StdReturnType MPU_srWriteRegister(INT8U u8RegisterAddr, INT8U u8RegisterValue)

DESCRIPTION :   used to write on MPU6050 Register

PARAMETER1  :   u8RegisterAddr:  Address of register to write over
PARAMETER2  :   u8RegisterValue: Value to be written

RETURN VALUE:   E_OK     : Write is done successfully
                E_NOT_OK : Something goes wrong

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_srWriteRegister(const INT8U u8RegisterAddr,const INT8U u8RegisterValue)
{
    I2C_Transaction MPU6050_I2CTransaction;
    INT8U           u8WriteBuffer[2] = {u8RegisterAddr, u8RegisterValue};
    StdReturnType   srWriteStatus = E_OK;

    MPU6050_I2CTransaction.slaveAddress = 0x68;
    MPU6050_I2CTransaction.writeCount   = 2U;
    MPU6050_I2CTransaction.writeBuf     = (void*)(u8WriteBuffer);
    MPU6050_I2CTransaction.readCount    = 0U;
    MPU6050_I2CTransaction.readBuf      = STD_NULL;
    if(I2C_transfer(MPU6050_I2CHandle, &MPU6050_I2CTransaction))
    {
        srWriteStatus = E_OK;
    }
    else
    {
        srWriteStatus = E_NOT_OK;
    }

return srWriteStatus;
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_srSetSleepMode                                 <MPU_CORE>

SYNTAX:         StdReturnType MPU_srSetSleepMode(const BOOLEAN bSleepState)

DESCRIPTION :   used to enable and disable sleep mode

PARAMETER1  :   bSleepState: sleep mode state

RETURN VALUE:   E_OK     : setting is done successfully
                E_NOT_OK : Something goes wrong

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_srSetSleepMode(const BOOLEAN bSleepState)
{
    INT8U         u8RegisterValue;
    StdReturnType srConfigStatus = E_OK;
    INT8U         u8SleepMask = 0U;

    u8SleepMask = (bSleepState == STD_TRUE)?PWR_SLEEP_MODE_B:0U;
    srConfigStatus = MPU_srReadRegister(PWR_MGMT_1_R, &u8RegisterValue, 1U);
    if(srConfigStatus == E_OK)
    {
        srConfigStatus = MPU_srWriteRegister(PWR_MGMT_1_R, SET_BIT(u8RegisterValue, PWR_SLEEP_MODE_B, u8SleepMask));
    }
    else
    {
        /*Log Error Event*/
    }

    return srConfigStatus;
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_srSetAccelerometerScale                              <MPU_CORE>

SYNTAX:         StdReturnType MPU_srSetAccelerometerScale(const MPU_enumAccelScaleType objenumAccelScaleType)

DESCRIPTION :   used to set accelerometer full scale range

PARAMETER1  :   objenumAccelScaleType: accelerometer full scale range to be set

RETURN VALUE:   E_OK     : setting is done successfully
                E_NOT_OK : Something goes wrong

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_srSetAccelerometerScale(const MPU_enumAccelScaleType objenumAccelScaleType)
{
    INT8U         u8RegisterValue;
    StdReturnType srConfigStatus = E_OK;

    srConfigStatus = MPU_srReadRegister(ACCEL_CONFIG_R, &u8RegisterValue, 1U);
    if(srConfigStatus == E_OK)
    {
        u8RegisterValue &=~ (ACCEL_SCALE_SEL_M);
        switch(objenumAccelScaleType)
        {
        case cs16gScaleRange:
            u8RegisterValue |= ACCEL_16G_SEL_M;
            break;
        case cs8gScaleRange:
            u8RegisterValue |= ACCEL_8G_SEL_M;
            break;
        case cs4gScaleRange:
            u8RegisterValue |= ACCEL_4G_SEL_M;
            break;
        case cs2gScaleRange:
        default:
            u8RegisterValue |= ACCEL_2G_SEL_M;
            break;
        }
        srConfigStatus = MPU_srWriteRegister(ACCEL_CONFIG_R, u8RegisterValue);
    }
    else
    {
        /*No Action*/
    }

    return srConfigStatus;
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_srSetGyroscopeScale                              <MPU_CORE>

SYNTAX:         StdReturnType MPU_srSetGyroscopeScale(const MPU_enumGyroScaleType objenumGyroScaleType)

DESCRIPTION :   used to set gyroscope full scale range

PARAMETER1  :   objenumGyroScaleType: gyroscope full scale range to be set

RETURN VALUE:   E_OK     : setting is done successfully
                E_NOT_OK : Something goes wrong

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_srSetGyroscopeScale(const MPU_enumGyroScaleType objenumGyroScaleType)
{
    INT8U         u8RegisterValue;
    StdReturnType srConfigStatus = E_OK;

    srConfigStatus = MPU_srReadRegister(GYRO_CONFIG_R, &u8RegisterValue, 1U);
    if(srConfigStatus == E_OK)
    {
        u8RegisterValue &=~ (GYRO_SCALE_SEL_M);
        switch(objenumGyroScaleType)
        {
        case cs2000ScaleRange:
            u8RegisterValue |= GYRO_2000_SEL_M;
            break;
        case cs1000ScaleRange:
            u8RegisterValue |= GYRO_1000_SEL_M;
            break;
        case cs500ScaleRange:
            u8RegisterValue |= GYRO_500_SEL_M;
            break;
        case cs250ScaleRange:
        default:
            u8RegisterValue |= GYRO_250_SEL_M;
            break;
        }
        srConfigStatus = MPU_srWriteRegister(GYRO_CONFIG_R, u8RegisterValue);
    }
    else
    {
        /*No Action*/
    }

    return srConfigStatus;
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_srReadTemperature                                <MPU_CORE>

SYNTAX:         StdReturnType MPU_srReadTemperature(FP32* const pF32MPUTemperature)

DESCRIPTION :   used to read temperature of MPU6050

PARAMETER1  :   pF32MPUTemperature:  pointer to variable that will contain MPU temperature

RETURN VALUE:   E_OK     : Read is done successfully
                E_NOT_OK : Something goes wrong

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_srReadTemperature(FP32* const pF32MPUTemperature)
{
    INT16U        u16RegisterValue;
    StdReturnType srReadStatus;

    srReadStatus = MPU_srReadRegister(TEMP_OUT_R, (INT8U*)&u16RegisterValue, 2U);
    if(srReadStatus == E_OK)
    {
        /*Register Swapping*/
        *pF32MPUTemperature = ((FP32)((INT16S)SWAP_VALUE16(u16RegisterValue)))/340.0 + 36.53;
    }
    else
    {
        /*log error: "failed to read temperature register"*/
    }

    return srReadStatus;
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_srReadAccelometerX                                <MPU_CORE>

SYNTAX:         StdReturnType MPU_srReadAccelometerX(FP32* const pF32AcceleometerX)

DESCRIPTION :   used to read acceleration in X direction

PARAMETER1  :   pF32MPUTemperature:  pointer to variable that will contain MPU acceleration in X direction

RETURN VALUE:   E_OK     : Read is done successfully
                E_NOT_OK : Something goes wrong

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_srReadAccelometerX(FP32* const pF32AcceleometerX)
{
    INT16U        u16RegisterValue;
    StdReturnType srReadStatus;
    FP32          FP32Sensitivity = objstrMPUConfigType.objstrAccelConfigType.FP32AccelSensitivity;

    srReadStatus = MPU_srReadRegister(ACCEL_XOUT_R, (INT8U*)&u16RegisterValue, 2U);
    if(srReadStatus == E_OK)
    {
        /*Register Swapping*/
        *pF32AcceleometerX = ((FP32)((INT16S)SWAP_VALUE16(u16RegisterValue)))/FP32Sensitivity;
    }
    else
    {
        /*log error: "failed to read temperature register"*/
    }

    return srReadStatus;
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_srReadAccelometerY                                <MPU_CORE>

SYNTAX:         StdReturnType MPU_srReadAccelometerY(FP32* const pF32AcceleometerY)

DESCRIPTION :   used to read acceleration in Y direction

PARAMETER1  :   pF32AcceleometerY:  pointer to variable that will contain MPU acceleration in Y direction

RETURN VALUE:   E_OK     : Read is done successfully
                E_NOT_OK : Something goes wrong

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_srReadAccelometerY(FP32* const pF32AcceleometerY)
{
    INT16U        u16RegisterValue;
    StdReturnType srReadStatus;
    FP32          FP32Sensitivity = objstrMPUConfigType.objstrAccelConfigType.FP32AccelSensitivity;

    srReadStatus = MPU_srReadRegister(ACCEL_YOUT_R, (INT8U*)&u16RegisterValue, 2U);
    if(srReadStatus == E_OK)
    {
        /*Register Swapping*/
        *pF32AcceleometerY = ((FP32)((INT16S)SWAP_VALUE16(u16RegisterValue)))/FP32Sensitivity;
    }
    else
    {
        /*log error: "failed to read temperature register"*/
    }

    return srReadStatus;
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_srReadAccelometerZ                                <MPU_CORE>

SYNTAX:         StdReturnType MPU_srReadAccelometerZ(FP32* const pF32AcceleometerZ)

DESCRIPTION :   used to read acceleration in Z direction

PARAMETER1  :   pF32AcceleometerZ:  pointer to variable that will contain MPU acceleration in Z direction

RETURN VALUE:   E_OK     : Read is done successfully
                E_NOT_OK : Something goes wrong

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_srReadAccelometerZ(FP32* const pF32AcceleometerZ)
{
    INT16U        u16RegisterValue;
    StdReturnType srReadStatus;
    FP32          FP32Sensitivity = objstrMPUConfigType.objstrAccelConfigType.FP32AccelSensitivity;

    srReadStatus = MPU_srReadRegister(ACCEL_ZOUT_R, (INT8U*)&u16RegisterValue, 2U);
    if(srReadStatus == E_OK)
    {
        /*Register Swapping*/
        *pF32AcceleometerZ = ((FP32)((INT16S)SWAP_VALUE16(u16RegisterValue)))/FP32Sensitivity;
    }
    else
    {
        /*log error: "failed to read temperature register"*/
    }

    return srReadStatus;
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_srReadGyroscopeX                                <MPU_CORE>

SYNTAX:         StdReturnType MPU_srReadGyroscopeX(FP32* const pF32GyroscopeX)

DESCRIPTION :   used to read acceleration in X direction

PARAMETER1  :   pF32MPUTemperature:  pointer to variable that will contain MPU acceleration in X direction

RETURN VALUE:   E_OK     : Read is done successfully
                E_NOT_OK : Something goes wrong

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_srReadGyroscopeX(FP32* const pF32GyroscopeX)
{
    INT16U        u16RegisterValue;
    StdReturnType srReadStatus;
    FP32          FP32Sensitivity = objstrMPUConfigType.objstrGyroConfigType.FP32GyroSensitivity;

    srReadStatus = MPU_srReadRegister(GYRO_XOUT_R, (INT8U*)&u16RegisterValue, 2U);
    if(srReadStatus == E_OK)
    {
        /*Register Swapping*/
        *pF32GyroscopeX = ((FP32)((INT16S)SWAP_VALUE16(u16RegisterValue)))/FP32Sensitivity;
    }
    else
    {
        /*log error: "failed to read temperature register"*/
    }

    return srReadStatus;
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_srReadGyroscopeY                                <MPU_CORE>

SYNTAX:         StdReturnType MPU_srReadGyroscopeY(FP32* const pF32GyroscopeY)

DESCRIPTION :   used to read acceleration in Y direction

PARAMETER1  :   pF32GyroscopeY:  pointer to variable that will contain MPU acceleration in Y direction

RETURN VALUE:   E_OK     : Read is done successfully
                E_NOT_OK : Something goes wrong

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_srReadGyroscopeY(FP32* const pF32GyroscopeY)
{
    INT16U        u16RegisterValue;
    StdReturnType srReadStatus;
    FP32          FP32Sensitivity = objstrMPUConfigType.objstrGyroConfigType.FP32GyroSensitivity;

    srReadStatus = MPU_srReadRegister(GYRO_YOUT_R, (INT8U*)&u16RegisterValue, 2U);
    if(srReadStatus == E_OK)
    {
        /*Register Swapping*/
        *pF32GyroscopeY = ((FP32)((INT16S)SWAP_VALUE16(u16RegisterValue)))/FP32Sensitivity;
    }
    else
    {
        /*log error: "failed to read temperature register"*/
    }

    return srReadStatus;
}
/* START FUNCTION DESCRIPTION ------------------------------------------------------------------------------------------
MPU_srReadGyroscopeZ                                <MPU_CORE>

SYNTAX:         StdReturnType MPU_srReadGyroscopeZ(FP32* const pF32GyroscopeZ)

DESCRIPTION :   used to read acceleration in Z direction

PARAMETER1  :   pF32GyroscopeZ:  pointer to variable that will contain MPU acceleration in Z direction

RETURN VALUE:   E_OK     : Read is done successfully
                E_NOT_OK : Something goes wrong

Note        :   Start here

END DESCRIPTION -------------------------------------------------------------------------------------------------------*/
StdReturnType MPU_srReadGyroscopeZ(FP32* const pF32GyroscopeZ)
{
    INT16U        u16RegisterValue;
    StdReturnType srReadStatus;
    FP32          FP32Sensitivity = objstrMPUConfigType.objstrGyroConfigType.FP32GyroSensitivity;

    srReadStatus = MPU_srReadRegister(GYRO_ZOUT_R, (INT8U*)&u16RegisterValue, 2U);
    if(srReadStatus == E_OK)
    {
        /*Register Swapping*/
        *pF32GyroscopeZ = ((FP32)((INT16S)SWAP_VALUE16(u16RegisterValue)))/FP32Sensitivity;
    }
    else
    {
        /*log error: "failed to read temperature register"*/
    }

    return srReadStatus;
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




