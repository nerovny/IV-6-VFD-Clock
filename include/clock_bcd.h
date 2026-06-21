//BCD handling definitions and functions header file

#include <stdint.h>

#define     DIGIT_BYTE_CLEAN        0xF; //0b1111
#define     INT_TO_BCD_8BIT(val)    ((uint8_t)(((val / 10) << 4) | (val % 10)))

// Global VFD signals propagation delay, us
#define		DIGIT_DELAY_US			50

// Sturtup counter delay, ms
#define 	DIGIT_STARTUP_DELAY		10

// Reset the position and move the "cursor" before the first digit.
void DigitBCDReset(void);

// Move the "cursor" before the first digit without the resetting
void DigitBCDEOL(int current_pos);

// Output 0's to all the digits.
void DigitBCDZero(void);

// Output the 6 digits:     HI:MD:LO
void DigitBCDPrint(unsigned int num_high, unsigned int num_middle, unsigned int num_low);

// Prints the two-digit BCD byte at the current position 
void DigitBCDPrintByte(uint8_t bcd_byte);

// Do the funny 1..99 startup counter
void DigitBCDStartupRoll(void);
