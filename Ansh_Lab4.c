// Ref :- Purdue University Notes

/*
 * timer_2.c
 *
 *  Created on: Sep 1, 2017
 *      Author: Ansh Joshi
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

#define LED_BASE GPIO_PORTF_BASE //redifined for gpio
#define rled GPIO_PIN_1 //pin 1 command redefine
uint8_t r = 0;

void Timer0IntHandler(void)//Interrupt Handler
{
// Clear the timer interrupt
TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
// Read the current state of the GPIO pin and
// write back the opposite state
    r ^= rled;
    GPIOPinWrite(LED_BASE,rled, r);
}
int main(void)
{

 //Set the clock to 80Mhz
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);//starts system clock

  /*
    No need to enable the button peripheral since it's the same as the LED
    in this case
  */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//GPIO perph on
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);//Set Output at port F
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//
  TimerDisable(TIMER0_BASE, TIMER_BOTH);
  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
  TimerLoadSet(TIMER0_BASE, TIMER_BOTH, 40000000);
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  TimerIntRegister(TIMER0_BASE,TIMER_BOTH, Timer0IntHandler);
  IntEnable(INT_TIMER0A);

  //IntMasterEnable();
  TimerEnable(TIMER0_BASE, TIMER_BOTH);


while(1){}

}







