/*
*-----------------------------------------------------------------------------------------------------------------

*-----------------------------------------------------------------------------------------------------------------
*/
#ifndef  __COMPILEREXT_H
#define  __COMPILEREXT_H

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                     INCLUDES
*-----------------------------------------------------------------------------------------------------------------*
*/
#define USE_BSD
#define TI_DRIVERS_UART_DMA 0
#ifndef TI_DRIVERS_UART_DMA
#define TI_DRIVERS_UART_DMA 0
#endif
#include <ti/sysbios/family/arm/m3/Hwi.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Log.h>


/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
//#include <ti/sysbios/hal/hwi.h>
#include "ti/mw/fatfs/ff.h"
#include <ti/sysbios/gates/GateMutexPri.h>
#include <ti/sysbios/gates/GateMutex.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/utils/Load.h>
#include <ti/sysbios/hal/Timer.h>

#include <ti/drivers/EMAC.h>
#include <ti/drivers/emac/EMACSnow.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SDSPI.h>
#include <ti/drivers/sdspi/SDSPITiva.h>
#include <ti/drivers/UART.h>

#if TI_DRIVERS_UART_DMA
    #include <ti/drivers/uart/UARTTivaDMA.h>
#else
    #include <Main Library/Environment/UARTTiva.h>
#endif
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CTiva.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPITivaDMA.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/USBMSCHFatFs.h>
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogTiva.h>


#include "utils/cmdline.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"
#include <file.h>


#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

/* TIVAWARE headers*/
#include <inc/hw_memmap.h>
#include "inc/hw_epi.h"
#include <inc/hw_types.h>
#include "inc/hw_sysctl.h"
#include <inc/hw_ints.h>
#include <inc/hw_gpio.h>
#include "inc/hw_uart.h"
#include "inc/hw_hibernate.h"
#include "inc/hw_can.h"

#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/epi.h"
#include <driverlib/gpio.h>
#include <driverlib/flash.h>
#include <driverlib/sysctl.h>
//#include <driverlib/i2c.h>
#include <driverlib/timer.h>
#include <driverlib/eeprom.h>
#include <driverlib/adc.h>
#include <driverlib/ssi.h>
#include <driverlib/udma.h>
#include <driverlib/pin_map.h>
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/hibernate.h"
#include "driverlib/can.h"
#include "driverlib/crc.h"

/* ARM comoiler headers*/
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


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
 * Disable/Enable the interrupt and it should be used with caution as if it may
 * lead to delay of jitter
 */
#define CMP_u32mIRQDisable()                   Hwi_disable()
#define CMP_u32mIRQRestore(u32InterruptFlags)  Hwi_restore(u32InterruptFlags)
#define CMP_vmIRQDisable(INTERRUPT_MASK)       INTERRUPT_MASK = Hwi_disable()
#define CMP_vmIRQRestore(INTERRUPT_MASK)       Hwi_restore(INTERRUPT_MASK)

/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                           G L O B A L  D A T A   T Y P E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/



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
*                                    G L O B A L  F U N C T I O N   P R O T O T Y P E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

#ifdef __cplusplus
extern "C" {
#endif

/* Put the functions here */

#ifdef __cplusplus
}
#endif


/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                     MODULE END
*-----------------------------------------------------------------------------------------------------------------*
*/
#endif



