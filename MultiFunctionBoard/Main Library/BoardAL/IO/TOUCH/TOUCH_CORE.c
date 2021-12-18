/*
*-----------------------------------------------------------------------------------------------------------------
* Filename         : TOUCH_CORE.c
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
#include "TOUCH_CORE.h"
#include "DEM_CORE.h"
#include "IOHWAB_DIGITAL.h"
#include "OS.h"
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
*                                    L O C A L  F U N C T I O N   P R O T O T Y P E S
*-----------------------------------------------------------------------------------------------------------------*
*/

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
/* START FUNCTION DESCRIPTION ******************************************************************************************
TOUCH_vInit                                           <TOUCH_CORE>

SYNTAX:         void TOUCH_vInit(void)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :   Start here

END DESCRIPTION *******************************************************************************************************/
void TOUCH_vInit(void)
{

}
/* START FUNCTION DESCRIPTION ******************************************************************************************
TOUCH_vProcess                               <TOUCH_CORE>

SYNTAX:         void TOUCH_vMonitor(void)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

Note        :   the process function shall be called inside infinite loop

END DESCRIPTION *******************************************************************************************************/
void TOUCH_vMonitor(void)
{
    INT8U u8TouchValue = STD_LOW;

    u8TouchValue = IOHWAB_u8GetDigitalValue(csTouchInputChannel);

    if(u8TouchValue == STD_LOW)
    {
        DEM_vLogActivity0("\nTouch Signal is Low");
    }
    else
    {
        DEM_vLogActivity0("\nTouch Signal is High");
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
