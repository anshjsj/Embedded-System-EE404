
//Switching between different colors


/*

Syntax and Structyre referred from:

https://sites.google.com/site/luiselectronicprojects/tutorials/tiva-tutorials/tiva-gpio/digital-input-with-interrupt

*/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.c"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.c"
#include "driverlib/gpio.h"


/*
  These defines help if you want to change the LED pin or the Button pin.

*/
#define LED_PERIPH SYSCTL_PERIPH_GPIOF
#define LED_BASE GPIO_PORTF_BASE
#define rled GPIO_PIN_1
#define bled GPIO_PIN_2
#define gled GPIO_PIN_3

#define Button_PERIPH SYSCTL_PERIPH_GPIOF
#define ButtonBase GPIO_PORTF_BASE
#define Button GPIO_PIN_4
#define ButtonInt GPIO_INT_PIN_4

// Defines the state of pins
uint8_t value=0;

int count;
uint8_t r = 0;
uint8_t b = 0xFF;
uint8_t g = 0;

void PortFIntHandler(){ //Interrupt Handler
uint32_t status=0;

  status = GPIOIntStatus(ButtonBase,true);// Returns the address which is interrupt enabled
  GPIOIntClear(ButtonBase,status);

  if((status & ButtonInt) == ButtonInt){
    //Then there was a Button pin interrupt
    uint8_t value=0;

    value= GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);
        count++;
        /*
        intiating count for switch
        */
    if( (value==0) & (count <= 10)){
      r ^= rled;
            GPIOPinWrite(LED_BASE,rled, r);
            b ^= bled;
            GPIOPinWrite(LED_BASE,bled, b);
            GPIOPinWrite(LED_BASE,gled, 0);
            for(int j =0;j<1000;j++);
            //Switching between red and blue
    }
        else if ((value==0) & (count >=10)){
            g = 0xFF;
            GPIOPinWrite(LED_BASE,rled, 0);
            GPIOPinWrite(LED_BASE,bled, 0);
            GPIOPinWrite(LED_BASE,gled, g);
        }// Turns on Green LED finally

  }

}

int main(void)
{

 //Set the clock to 80Mhz
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

  /*
    No need to enable the button peripheral since it's the same as the LED
    in this case
  */
  SysCtlPeripheralEnable(LED_PERIPH);
  SysCtlDelay(3);

  /*
    Configure the switch on the left of the launchpad, GPIO_PIN_4 to a input with
    internal pull-up.
  */
  GPIOPinTypeGPIOInput(ButtonBase, Button);
  GPIOPadConfigSet(ButtonBase ,Button,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);//Pull-up
  GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE);// Type of Interrupt
  GPIOIntRegister(GPIO_PORTF_BASE,PortFIntHandler);
  GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);//Interrupt Enable

  /*
    Configures the Red LED, GPIO_PIN_1, to output
  */
  GPIOPinTypeGPIOOutput(LED_BASE, rled|bled|gled);

while(1){}

}



