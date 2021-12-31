/*
*-----------------------------------------------------------------------------------------------------------------*
*                                                     INCLUDES
*-----------------------------------------------------------------------------------------------------------------*
*/
#include <Main Library/Environment/COMPILER_EXT.h>
#include "STD_TYPE.h"

/*
*-----------------------------------------------------------------------------------------------------------------*
*---------------------------------------------------------------------------------------------------------------
*                                    L O C A L  F U N C T I O N   P R O T O T Y P E S
*---------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------*
*/
static bool sign(float n);
static void reverse(char* str, int len);
static int intToStr(int x, char str[], int d);
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

/*-----------------------------------------------------------------------------------------------------------------------*/
/*! fn        void UTIL_vSwapBytes(INT8U *pu8BytesUnderSwaping, INT8U u8NumberOfBytes)

@brief   the function is reponsable to swap bytes

@param[in]  :   pu8BytesUnderSwaping : pointer to the data to be swaped

@param[in]  :   u8NumberOfBytes      : Number of bytes to be swaped

@return   N/A

@note        :   NA

-----------------------------------------------------------------------------------------------------------------------------*/
void UTIL_vSwapBytes(INT8U *pu8BytesUnderSwaping, INT8U u8NumberOfBytes)
{
    INT8U         u8Tempbyte;

    u8Tempbyte = pu8BytesUnderSwaping[0];
    switch(u8NumberOfBytes)
    {
    case 2:
         pu8BytesUnderSwaping[0] = pu8BytesUnderSwaping[1];
         pu8BytesUnderSwaping[1] = u8Tempbyte;
        break;
    case 4:
        pu8BytesUnderSwaping[0] = pu8BytesUnderSwaping[1];
        pu8BytesUnderSwaping[1] = pu8BytesUnderSwaping[2];
        pu8BytesUnderSwaping[2] = pu8BytesUnderSwaping[3];
        pu8BytesUnderSwaping[3] = u8Tempbyte;
        break;
    }

}
 // Converts a floating-point/double number to a string.
void UTIL_ftoa(float n, char* res, int afterpoint)
{
  bool bnsign = sign(n);

  if((n!=0)&&(bnsign==false))
  {
      res[0]='-';
      res++;
  }
  n = fabsf(n);
  // Extract integer part
  int ipart = (int)n;

  // Extract floating part
  float fpart = n - (float)ipart;

  // convert integer part to string
  int i = intToStr(ipart, res, 0);

  // check for display option after point
  if (afterpoint != 0) {
      res[i] = '.'; // add dot

      // Get the value of fraction part upto given no.
      // of points after dot. The third parameter
      // is needed to handle cases like 233.007
      fpart = fpart * pow(10, afterpoint);

      intToStr((int)fpart, res + i + 1, afterpoint);
  }
}

// Reverses a string 'str' of length 'len'
static void reverse(char* str, int len)
{
 int i = 0, j = len - 1, temp;
 while (i < j) {
     temp = str[i];
     str[i] = str[j];
     str[j] = temp;
     i++;
     j--;
 }
}

// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
static int intToStr(int x, char str[], int d)
{
 int i = 0;
 while (x) {
     str[i++] = (x % 10) + '0';
     x = x / 10;
 }

 // If number of digits required is more, then
 // add 0s at the beginning
 while (i < d)
     str[i++] = '0';

 reverse(str, i);
 str[i] = '\0';
 return i;
}

static bool sign(float n)
{
 if(n >= 0)
     return true;
 else
     return false;
}
#define UTIL_vmFfomMSToCycleInHZ(ui32ClockFreq, ui32Time_uS)                       \
    (((ui32Time_uS) == 0) ? 0 :                                               \
    (((ui32ClockFreq) / 1000000) * ((((ui32Time_uS) * 1000) - 1) / 1000)) + 1)



