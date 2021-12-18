/*
*-----------------------------------------------------------------------------------------------------------------

*-----------------------------------------------------------------------------------------------------------------
*/

#ifndef  STD_TYPE_H
#define  STD_TYPE_H
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                     INCLUDES
*-----------------------------------------------------------------------------------------------------------------*
*/


/*
*-----------------------------------------------------------------------------------------------------------------*
*-----------------------------------------------------------------------------------------------------------------
*                                              # G L O B A L  M A C R O S
*-----------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

/*
*-----------------------------------------------------------------------------------------------------------------*
*-----------------------------------------------------------------------------------------------------------------
*                                           G L O B A L  D A T A   T Y P E S
*-----------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
    typedef unsigned char        INT8U;                    /* Unsigned  8 bit quantity                           */
	typedef          char        INT8S;                    /* Signed    8 bit quantity                           */
    typedef unsigned short       INT16U;                   /* Unsigned 16 bit quantity                           */
    typedef signed   short       INT16S;                   /* Signed   16 bit quantity                           */
    typedef unsigned int         INT32U;                   /* Unsigned 32 bit quantity                           */
    typedef signed   int         INT32S;                   /* Signed   32 bit quantity                           */
    typedef unsigned long long   INT64U;                   /* Unsigned 64 bit quantity                           */
    typedef signed   long long   INT64S;                   /* Signed   64 bit quantity                           */
    typedef float                FP32;                     /* Single precision floating point                    */
    typedef double               FP64;                     /* Double precision floating point                    */
    typedef unsigned char        BOOLEAN;


typedef INT8U StdReturnType;


/*
*-----------------------------------------------------------------------------------------------------------------*
*-----------------------------------------------------------------------------------------------------------------
*                                                   # D E F I N E S
*-----------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/

/*
 * Standard return type
 */
#define E_OK                           (StdReturnType)0x00
#define E_NOT_OK                       (StdReturnType)0x01

#define STD_HIGH        0x01
#define STD_LOW         0x00

#define STD_ACTIVE      0x01
#define STD_IDLE        0x00

#define STD_TRUE        (BOOLEAN)0x01
#define STD_FALSE       (BOOLEAN)0x00

#define STD_ON          0x01
#define STD_OFF         0x00

#define STD_NULL                        (void*)0
/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                     MODULE END
*-----------------------------------------------------------------------------------------------------------------*
*/

#endif /* end #ifndef  STD_TYPE_H */


