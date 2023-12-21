#include <stdint.h>

int main(void)
{
	//	zmienne wskaźnikowe zwykle rozpoczynają się od małej litery p - dobra praktyka

		uint32_t *pClkCtrlReg = (uint32_t*)0x4002104C;
		uint32_t *pPortBModeReg = (uint32_t*)0x48000400;
		uint32_t *pPortBOutReg = (uint32_t*)0x48000414;

		uint32_t *pPortAModeReg = (uint32_t*)0x48000000;
		uint32_t *pPortAOutReg = (uint32_t*)0x48000014;

//1. enable the clock for GPOIB & GPOIA peripheral in the AHB2ENR (SET the 1st bit position)
	*pClkCtrlReg |= ( 1 << 1);			// GPOIB
	*pClkCtrlReg |= ( 1 << 0);			// GPOIA

//2. configure the mode of the IO pin as output
	// a) GPIOB - PB_3 - D13
	*pPortBModeReg &= ~( 3 << 6);		//a. clear the 6th and 7th bit positions (CLEAR)
	*pPortBModeReg |= ( 1 << 6);		//b. make 6th bit position as 1 (SET)

	// b) GPIO B - PB_0 - D3
	*pPortBModeReg &= ~( 3 << 0);		//a. clear the 0 and 2nd bit positions (CLEAR)
	*pPortBModeReg |= ( 1 << 0);		//b. make 0th bit position as 1 (SET)

	// c) GPIO B - PB_7 - D4
	*pPortBModeReg &= ~( 3 << 14);		//a. clear the 14th and 15th bit positions (CLEAR)
	*pPortBModeReg |= ( 1 << 14);		//b. make 14th bit position as 1 (SET)

	// d) GPIO A - PA_12 - D2
	*pPortAModeReg &= ~( 3 << 24);		//a. clear the 24th and 25th bit positions (CLEAR)
	*pPortAModeReg |= ( 1 << 24);		//b. make 24th bit position as 1 (SET)

//3.SET 12th bit of the output data register to make I/O pin-3 as HIGH
	*pPortBOutReg |= ( 1 << 3);	// PB_3 - D13
	*pPortBOutReg |= ( 1 << 0);	// PB_0 - D3
	*pPortBOutReg |= ( 1 << 7); // PB_7 - D4
	*pPortAOutReg |= ( 1 << 12); // PA_12 - D2

	// D2 - PA_12
	// D3 - PB_0
	// D4 - PB_7
	// D13 - PB_3
    while(1);
}
