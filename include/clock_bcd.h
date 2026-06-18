/*
*   @file   clock_bcd.h
*   @brief  BCD handling definitions and functions header file
*/

#define     DIGIT_BYTE_CLEAN    0xF; //0b1111

// Move the "cursor" before the first digit.
void DigitBCDReset(void);

// Output 0's to all the digits.
void DigitBCDZero(void);