/*
*-----------------------------------------------------------------------------------------------------------------

*
*-----------------------------------------------------------------------------------------------------------------
*/


#ifndef  IOHWAB_DIGITAL_H
#define  IOHWAB_DIGITAL_H

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
#include <Main Library/Environment/COMPILER_EXT.h>
#include "STD_TYPE.h"

/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                                   # D E F I N E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
/*
 ==================================================================================================================
 *                                                  VERSION NUMBER
 ==================================================================================================================
 */

/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                              # G L O B A L  M A C R O S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

/*
 ==================================================================================================================
 *                                                  OPTO
 ==================================================================================================================
 */

/*
 ==================================================================================================================
 *                                                  Strobe control
 ==================================================================================================================
 */

/*
 ==================================================================================================================
 *                                                  DIP Switch
 ==================================================================================================================
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
    csTouchInputChannel = 0U,
    csMaxNumberDigitalSignal
}IOHWAB_enumDigitalSignalType;
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
INT8U   IOHWAB_u8GetDigitalValue(IOHWAB_enumDigitalSignalType objenumDigitalChannel);
#ifdef __cplusplus
}
#endif


/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                     MODULE END
*-----------------------------------------------------------------------------------------------------------------*
*/
#endif /* end #ifndef  IOHWAB_DIGITAL_H */



