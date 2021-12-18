/*
*-----------------------------------------------------------------------------------------------------------------
* Filename         : PORT_CORE.c
* Version          : 0.0.0
* Created          : 2021/04/10
* Author           : Ayman Ashraf
*-----------------------------------------------------------------------------------------------------------------
*
*-----------------------------------------------------------------------------------------------------------------
*/


/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                                   # I N C L U D E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
#include "PORT_CORE.h"


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
/* I2C objects */
I2CTiva_Object   i2cTivaobjects[1];
SDSPITiva_Object sdspiTivaobjects;

/* I2C configuration structure, describing which pins are to be used */
const I2CTiva_HWAttrs i2cTivaHWAttrs[] = {
    {
        .baseAddr = I2C1_BASE,
        .intNum = INT_I2C1,
        .intPriority = (~0)
    }
};

const SDSPITiva_HWAttrs sdspiTivaHWattrs = {
                                                SSI1_BASE,          /* SPI base address */
                                                GPIO_PORTD_BASE,    /* SPI SCK PORT */
                                                GPIO_PIN_0,         /* SCK PIN */
                                                GPIO_PORTD_BASE,    /* SPI MISO PORT*/
                                                GPIO_PIN_2,         /* MISO PIN */
                                                GPIO_PORTD_BASE,    /* SPI MOSI PORT */
                                                GPIO_PIN_3,         /* MOSI PIN */
                                                GPIO_PORTD_BASE,    /* GPIO CS PORT */
                                                GPIO_PIN_1,         /* CS PIN */
                                           };
const SDSPI_Config SDSPI_config[] = {
    {&SDSPITiva_fxnTable, &sdspiTivaobjects, &sdspiTivaHWattrs},
    {STD_NULL, STD_NULL, STD_NULL}
};
const I2C_Config I2C_config[] = {
    {&I2CTiva_fxnTable, &i2cTivaobjects[0], &i2cTivaHWAttrs[0]},
    {STD_NULL, STD_NULL, STD_NULL}
};
/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                    L O C A L  F U N C T I O N   P R O T O T Y P E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
static void PORT_vInitGeneralIOPort(void);
static void PORT_vInitGPIO(void);
static void PORT_vInitI2C(void);
static void PORT_vInitSDCardPort(void);
/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                              G L O B A L   V A R I A B L E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
INT32U PORT_u32ProcessorFrequency;
/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                             G L O B A L   F U N C T I O N S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

/* START FUNCTION DESCRIPTION ******************************************************************************************
PORT_vInit                                           <PORT_CORE>

SYNTAX:         void PORT_vInit(void)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :   Start here

END DESCRIPTION *******************************************************************************************************/
void PORT_vInit(void)
{
    Types_FreqHz strFrequencyHz;

    /* get current processor frquency */
    BIOS_getCpuFreq(&strFrequencyHz);
    PORT_u32ProcessorFrequency = strFrequencyHz.lo;
    PORT_vInitGeneralIOPort();
    PORT_vInitGPIO();
    PORT_vInitI2C();
    PORT_vInitSDCardPort();
    return;
}
/* START FUNCTION DESCRIPTION ******************************************************************************************
PORT_vProcess                                        <PORT_CORE>

SYNTAX:         void PORT_vProcess(void)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :   the process function shall be called inside infinite loop

END DESCRIPTION *******************************************************************************************************/
void PORT_vProcess(void)
{
    /* here we shall do the repeated tasks*/
}


/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                               L O C A L   F U N C T I O N S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
/* START FUNCTION DESCRIPTION ******************************************************************************************
PORT_vInitGeneralIOPort                                        <PORT_CORE>

SYNTAX:         void PORT_vInitGeneralIOPort(void)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :

END DESCRIPTION *******************************************************************************************************/
static void PORT_vInitGeneralIOPort(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}
/* START FUNCTION DESCRIPTION ******************************************************************************************
PORT_vInitGPIO                                        <PORT_CORE>

SYNTAX:         void PORT_vInitGPIO(void)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :

END DESCRIPTION *******************************************************************************************************/
static void PORT_vInitGPIO(void)
{
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_3);    /* Set PA3 as I/P, Touch Input */
    MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
}
/* START FUNCTION DESCRIPTION ******************************************************************************************
PORT_vInitGPIO                                        <PORT_CORE>

SYNTAX:         void PORT_vInitI2C(void)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :

END DESCRIPTION *******************************************************************************************************/
static void PORT_vInitI2C(void)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1); /* Enable I2C1 Peripheral Clocks */
    MAP_GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    MAP_GPIOPinConfigure(GPIO_PA7_I2C1SDA);
    MAP_GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);  /*I2C1 SCL*/
    MAP_GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);    /*I2C1 SDA*/
    I2C_init();
}
/* START FUNCTION DESCRIPTION ******************************************************************************************
PORT_vInitSDCardPort                                        <PORT_CORE>

SYNTAX:         void PORT_vInitSDCardPort()

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :

END DESCRIPTION *******************************************************************************************************/
static void PORT_vInitSDCardPort(void)
{
    /* Enable the peripherals used by the SD Card */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);

    /* Configure pad settings */
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    GPIOPinConfigure(GPIO_PD0_SSI1CLK);     /* Enable pin PD0 for SSI1 SSI1CLK, clock of the SD card */
    GPIOPinConfigure(GPIO_PD2_SSI1RX);      /* Enable pin PD2 for SSI1 SSI1XDAT1, MISO(RX) to the SD card*/
    GPIOPinConfigure(GPIO_PD3_SSI1TX);      /* Enable pin PD3 for SSI1 SSI1XDAT0, MOSI(TX) to the SD card */
    GPIOPinConfigure(GPIO_PD1_SSI3FSS);      /* Enable pin PD1 for SSI1 SSI3FSS, Chip select of the SD card */

    //SDSPI_init();
}
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                      MODULE END
*-----------------------------------------------------------------------------------------------------------------*
*/




