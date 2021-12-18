/*
*-----------------------------------------------------------------------------------------------------------------
* Filename         : IOHWAB_DIGITAL.c
* Version          : 0.0.0
* Created          : 2021/04/10
* File Description : this is the core module of IOHWAB that will hold all the basic functionalities of the module
* Modifications    :
* Author:   Date:2021/10/04                  Rev:0                  Task ID:
* - Created File structure templates according to the General Requirement of Embedded Software Modules document
*   (ESWM SRS).
* Author:                   Date:                            Rev:                   Task ID:
* - list the modifications here
* - list the modifications here
* - list the modifications here
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
#include "PORT_CORE.h"
#include "IOHWAB_DIGITAL.h"
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
IOHWAB_u8GetDigitalValue                           <IOHWAB_DIGITAL>

SYNTAX:         INT8U IOHWAB_u8GetDigitalValue(void)

DESCRIPTION :   Start here

PARAMETER1  :   Start here

RETURN VALUE:   Start here

END DESCRIPTION *******************************************************************************************************/
INT8U IOHWAB_u8GetDigitalValue(IOHWAB_enumDigitalSignalType objenumDigitalChannel)
{
    INT8U u8DigitalValue = STD_LOW;

    switch(objenumDigitalChannel)
    {
    case csTouchInputChannel:
        u8DigitalValue = (INT8U)PORT_u32mGetTouchState();
        break;
    default:
        break;
    }
    return u8DigitalValue;
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