// put implementations for functions, explain how it works
// put your names here, date

// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 3/6/2015 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly


#include <stdint.h>
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ 
	/*SYSCTL_RCGCGPIO_R |= 0x01;
  while((SYSCTL_PRGPIO_R&0x01) != 0x01){};
	GPIO_PORTE_DIR_R |=  0xEC;      //make PA23567
  GPIO_PORTE_DEN_R |= 0xEC; */
	
	SYSCTL_RCGCGPIO_R |= 0x10;
  while((SYSCTL_PRGPIO_R&0x10) != 0x10){};
  //delay = SYSCTL_RCGCGPIO_R;      //   allow time for clock to stabilize
  //delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTE_DIR_R &= ~0x10;      // 2) make PE4 input
  GPIO_PORTE_AFSEL_R |= 0x10;     // 3) enable alternate function on PE4
  GPIO_PORTE_DEN_R &= ~0x10;      // 4) disable digital I/O on PE4
  GPIO_PORTE_AMSEL_R |= 0x10;     // 5) enable analog functionality on PE4
  SYSCTL_RCGCADC_R |= 0x0001;   // 7) activate ADC0 
		
	//initailize PE0-1 as inputs for debugging?
	GPIO_PORTE_DATA_R &= ~0x02;		//clear PE1
	GPIO_PORTE_DIR_R |=  0x02;      //make PE1 output
  GPIO_PORTE_DEN_R |= 0x02;    		
	
   		
//  while((SYSCTL_PRADC_R&0x0001) != 0x0001){};    // good code, but not yet implemented in simulator
  //delay = SYSCTL_RCGCADC_R;         // extra time for clock to stabilize
  //delay = SYSCTL_RCGCADC_R;         // extra time for clock to stabilize
  //delay = SYSCTL_RCGCADC_R;         // extra time for clock to stabilize
  //delay = SYSCTL_RCGCADC_R;

  ADC0_PC_R &= ~0xF;              // 7) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;       // 11) clear SS3 field
  ADC0_SSMUX3_R += 9;             //    set channel
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3

}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
	int data;
	ADC0_PSSI_R = 0x0008; 	//initiates a software start to SS3 (hopefully allows for keil debugging)
	while ((ADC0_RIS_R & 0x08) == 0){};	//wait for interrupt to be triggered
		//could have if(((ADC0_RIS_R & 0x08) == 0) instead to allow for hearbeat and so program stays on .025 second loop
	data = ADC0_SSFIFO3_R & 0xFFF;	
	ADC0_ISC_R = 0x0008; 	//acknowledge completion by clearing IRS bit, getting it ready to be set by the next interrupt
  return data; // remove this, replace with real code
}


