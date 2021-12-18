/*
*-----------------------------------------------------------------------------------------------------------------

*-----------------------------------------------------------------------------------------------------------------
*/

#ifndef  __VERBOSE_H
#define  __VERBOSE_H



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
#include "COMPILER_EXT.h"
/*
 ==================================================================================================================
 *                                                  Application Include
 ==================================================================================================================
 */
#include "STD_TYPE.h"

/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                                   # D E F I N E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
//#define USE_CODE_COMPOSER_CONSOLE
//#define USER_UART_CONSOLE

/*
 * define verbose level in the module
 */
#define VERBOSE_LEVEL_0             0      /* Most verbose details */
#define VERBOSE_LEVEL_1             1
#define VERBOSE_LEVEL_2             2
#define VERBOSE_LEVEL_3             3
#define VERBOSE_LEVEL_4             4
#define VERBOSE_LEVEL_5             5      /* LEAST verbose details */
extern BOOLEAN VERBOSE_bSTarted;
/* the enable support for gcc extension*/
/* Note 960: Violates MISRA 2004 Required Rule 19.10, unparenthesized macro parameter in definition of macro*/
/*lint -save -esym(960,19.10)*/
#define mVerbose(fmt, ...)\
    if(VERBOSE_bSTarted == STD_TRUE)\
    {\
        (VERBOSE_printf("%s:%d: " fmt"\n", __FILE__, __LINE__, ##__VA_ARGS__));\
    }\
    else\
    {\
          (System_printf("%s:%d: " fmt"\n", __FILE__, __LINE__, ##__VA_ARGS__));\
          (System_flush());\
    }
#define mVerboseNoLine(fmt, ...)\
        if(VERBOSE_bSTarted == STD_TRUE)\
        {\
            (VERBOSE_printf(fmt, ##__VA_ARGS__));\
        }\
        else\
        {\
          (System_printf(fmt, ##__VA_ARGS__));\
          (System_printf("\n"));\
          (System_flush());\
        }

/*lint -restore */


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
void VERBOSE_vInit(void);
void VERBOSE_printf(const char *pcString, ...);
#ifdef __cplusplus
}
#endif


/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                     MODULE END
*-----------------------------------------------------------------------------------------------------------------*
*/
#endif


