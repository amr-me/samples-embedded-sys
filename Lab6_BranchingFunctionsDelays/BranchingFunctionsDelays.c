// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 8, 2014

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"

//#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
//#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
//#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
//#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
//#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
//#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
//#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
//#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
//#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define PF4                     (*((volatile unsigned long *)0x40025040))
#define PF3                     (*((volatile unsigned long *)0x40025020))
#define PF2                     (*((volatile unsigned long *)0x40025010))
#define PF1                     (*((volatile unsigned long *)0x40025008))
#define PF0                     (*((volatile unsigned long *)0x40025004))
#define GPIO_PORTF_DR2R_R       (*((volatile unsigned long *)0x40025500))
#define GPIO_PORTF_DR4R_R       (*((volatile unsigned long *)0x40025504))
#define GPIO_PORTF_DR8R_R       (*((volatile unsigned long *)0x40025508))
#define GPIO_LOCK_KEY           0x4C4F434B  // Unlocks the GPIO_CR register
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

//unsigned long In; // input from PF4
//unsigned long Out; // output to PF2 (blue LED)

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PortF_Init(void);
void delay_ms(unsigned long ms);
void Toggle_PF(void);

int main(void){ unsigned long volatile delay;
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
  // initialization goes here
	PortF_Init();
	
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
    // body goes here
		// PF2 = 1   100
		
		// if PF4 = 0 toggle PF2; else PF2 solid
		//    00010000
		if ((GPIO_PORTF_DATA_R & 0x10) == 0)
		{
			Toggle_PF();
		}
		else
			GPIO_PORTF_DATA_R |= (1 << 2);
  }
}


void PortF_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000020;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
 // GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
	// PF2 output , PF4 Input         000000100
	GPIO_PORTF_DIR_R = 0x04;
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
	//enable PUR for PF4 only 
  GPIO_PORTF_PUR_R = 0x10;         
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
	// PF2 on
	GPIO_PORTF_DATA_R |= (1 << 2);
}


// delay function declaration
void delay_ms(unsigned long i)
{
	unsigned long j = i*16000;
	while (j > 0)
	{
		j--;
	}
}


void Toggle_PF(void)
{
		// toggle function
		GPIO_PORTF_DATA_R |= (1 << 2);
		// 100ms delay
		delay_ms(100);
		GPIO_PORTF_DATA_R &= ~(1 << 2);
		delay_ms(100);
}