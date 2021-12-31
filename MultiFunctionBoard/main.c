/*
* main.c
* Version  : 0.0.0
* Updated  : 2021/12/31
* Author   : Ayman Ashraf
*/

/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                   # I N C L U D E S
*-----------------------------------------------------------------------------------------------------------------*
*/
#include <ESP8266_MAIN.h>
#include "TOUCH_MAIN.h"
#include "MPU_MAIN.h"
#include <Main Library/Environment/uartstdio.h>
#include "PORT_CORE.h"
#include "DEM_OS.h"
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                             M A I N   F U N C T I O N
*-----------------------------------------------------------------------------------------------------------------*
*/
int main(void)
{
    PORT_vInit();
    ESP8266_vInit();
    MPU_vInit();
    TOUCH_vInit();
    DEM_vInitOS();
    BIOS_start();    /* does not return */

    return 0;
}
