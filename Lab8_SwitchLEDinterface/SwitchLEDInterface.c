// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// December 28, 2014
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
//#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
	
unsigned long in,out;


// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PortE_Init(void);
void delay_ms(unsigned long i);
void Toggle_PF(void);

// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1);  // activate grader and set system clock to 80 MHz
  
	PortE_Init();
	
  EnableInterrupts();           // enable interrupts for the grader
  while(1)
	{
		// if PE0 == 1 ; toggle PE1
		if ((GPIO_PORTE_DATA_R & 0x01) == 0x01)
		{
			Toggle_PF();
		}
		// Else PE1 = 1
		else
		{
			GPIO_PORTE_DATA_R |= (1<<1);
		}
  }
  
}


void PortE_Init(void)
{
	volatile unsigned long delay;
	// clock port E        
	// 00000010000
	SYSCTL_RCGC2_R |= 0x00000010;
	delay = SYSCTL_RCGC2_R;
//	GPIO_PORTE_CR_R = 0x1F;
	GPIO_PORTE_AMSEL_R = 0x00;
	GPIO_PORTE_PCTL_R = 0x00000000;
	GPIO_PORTE_AFSEL_R = 0x00; 
	GPIO_PORTE_DEN_R = 0xFF;
	// PE0 input
	GPIO_PORTE_DIR_R &= ~(1<<0);
	// PE1 output
	GPIO_PORTE_DIR_R |= (1<<1);
	// init PE1 = 1
	GPIO_PORTE_DATA_R |= (1<<1);
	// delay 100 ms
	delay_ms(100);
}

void Toggle_PF(void)
{
		// toggle function
		GPIO_PORTE_DATA_R |= (1 << 1);
		// 100ms delay
		delay_ms(100);
		GPIO_PORTE_DATA_R &= ~(1 << 1);
		delay_ms(100);
}

void delay_ms(unsigned long i)
{
	unsigned long j = i*16000;
	while (j > 0)
	{
		j--;
	}
}

