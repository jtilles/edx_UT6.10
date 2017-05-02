// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

#define BIT0 	0x01
#define BIT1 	0x02
#define BIT2	0x04
#define BIT3	0x08
#define BIT4	0x10
#define BIT5	0x20
#define BIT6	0x40
#define BIT7	0x80

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); 			// Disable interrupts
void EnableInterrupts(void);  			// Enable interrupts
void initHardware(void);						// Sets up the ports to interact with the hardware
void initSysTick(void);							// Set up the systick timer
void delayMilliSec(uint16_t delay);	// Delay set number of milliseconds

// ***** 3. Subroutines Section *****

int main(void){ 
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
 
  
  EnableInterrupts();
  while(1){
     
  }
}

void initHardware(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= BIT1 | BIT4 | BIT5;     // 1) activate clock for Ports B,E, and F
  delay = SYSCTL_RCGC2_R;           				// allow time for clock to start
  
	GPIO_PORTB_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port B
	GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port E
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
	
	GPIO_PORTB_CR_R = 0x1F;           // allow changes to PB7-0
  GPIO_PORTE_CR_R = 0x1F;           // allow changes to PE7-0
	GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF7-0
  
	GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog on PB
	GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog on PE
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
	
	GPIO_PORTB_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PB7-0
	GPIO_PORTE_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PE7-0
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF7-0
	
  GPIO_PORTB_DIR_R = 0x3F;          								// 5) PB0-5 outputs
	GPIO_PORTE_DIR_R &= ~(BIT0 | BIT1 | BIT2);        // 5) PE0-2 Inputs
	GPIO_PORTF_DIR_R = BIT1 | BIT3;         					// 5) PF3,1 outputs
	
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) disable alt funct on PB7-0
	GPIO_PORTE_AFSEL_R = 0x00;        // 6) disable alt funct on PE7-0
	GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
	
  GPIO_PORTE_PDR_R = 0x07;          // enable pull-downs on PE0-2
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}

// Initialize the systick timer for precise timing applications
void initSysTick(void){
	NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;        // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it             
  NVIC_ST_CTRL_R = BIT2 | BIT0;          // enable SysTick with system clock (80 MHz)
}

/* Precise delays using systick timer
// Has a known number of system clock cycles neccesary to equal 1ms (timeConst)
//	counts that number of cycles the number of times specified by the input var "delay"
*/
void delayMilliSec(uint16_t delay){
	uint16_t timeConst = 800;		// Number of ticks of 80 MHz clock to equal 1ms
	while (delay){
		while(timeConst)
			timeConst--;
		delay--;
	}
}	