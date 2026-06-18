/*
*   @file   clock_bcd.h
*   @brief  BCD handling definitions and functions header file
*/

#include <stdint.h>

#define     DIGIT_BYTE_CLEAN        0xF; //0b1111
#define     INT_TO_BCD_8BIT(val)    ((uint8_t)(((val / 10) << 4) | (val % 10)))

// Move the "cursor" before the first digit.
void DigitBCDReset(void);

// Output 0's to all the digits.
void DigitBCDZero(void);

// Output the 6 digits:     HI:MD:LO
void DigitBCDPrint(unsigned int num_high, unsigned int num_middle, unsigned int num_low);

// Prints the two-digit BCD byte at the current position 
void DigitBCDPrintByte(uint8_t bcd_byte);
