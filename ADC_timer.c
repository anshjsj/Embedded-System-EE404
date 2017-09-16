

#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/pwm.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"
#include "driverlib/adc.h"
#include "utils/uartstdio.h"
#include "utils/uartstdio.c"
#include <string.h>
#include "driverlib/timer.h"
//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************

uint32_t ADCValues[1];

        //
        // These variables are used to store the temperature conversions for
        // Celsius and Fahrenheit.
        //
uint32_t TempValueC ;
uint32_t TempValueF ;

void
InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    //
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    //
    //
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    //
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

void Timer0IntHandler(void)//Interrupt Handler
{
// Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Ack Interrupt
    ADCIntClear(ADC0_BASE, 1);
    // Read ADC Data
    ADCSequenceDataGet(ADC0_BASE, 1, ADCValues);
    TempValueC = (uint32_t)(147.5 - ((75.0*3.3 *(float)ADCValues[0])) / 4096.0);
    TempValueF = ((TempValueC * 9) + 160) / 5;

                //
                // Display the temperature value on the console.
                //
                UARTprintf("Temperature = %3d*C or %3d*F\r", TempValueC,
                        TempValueF);

}

int main(){

      SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
      InitConsole();
      //
        // This array is used for storing the data read from the ADC FIFO. It
        // must be as large as the FIFO for the sequencer in use.  This example
        // uses sequence 3 which has a FIFO depth of 1.  If another sequence
        // was used with a deeper FIFO, then the array size must be changed.
        //


        //
        // Display the setup on the console.
        //
        UARTprintf("ADC ->\n");
        UARTprintf("  Type: Internal Temperature Sensor\n");
        UARTprintf("  Samples: One\n");
        UARTprintf("  Update Rate: 250ms\n");
        UARTprintf("  Input Pin: Internal temperature sensor\n\n");

        //
        // The ADC0 peripheral must be enabled for use.
        //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
        SysCtlDelay(3);

        //
        // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
        // will do a single sample when the processor sends a singal to start the
        // conversion.  Each ADC module has 4 programmable sequences, sequence 0
        // to sequence 3.  This example is arbitrarily using sequence 3.
        //
        ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);

        //
        // Configure step 0 on sequence 3.  Sample the temperature sensor
        // (ADC_CTL_TS) and configure the interrupt flag (ADC_CTL_IE) to be set
        // when the sample is done.  Tell the ADC logic that this is the last
        // conversion on sequence 3 (ADC_CTL_END).  Sequence 3 has only one
        // programmable step.  Sequence 1 and 2 have 4 steps, and sequence 0 has
        // 8 programmable steps.  Since we are only doing a single conversion using
        // sequence 3 we will only configure step 0.  For more information on the
        // ADC sequences and steps, reference the datasheet.
        //
        ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS | ADC_CTL_IE |
                                 ADC_CTL_END);

        //
        // Since sample sequence 3 is now configured, it must be enabled.
        //
        ADCSequenceEnable(ADC0_BASE, 3);

        //
        // Clear the interrupt status flag.  This is done to make sure the
        // interrupt flag is cleared before we sample.
        //
        ADCIntClear(ADC0_BASE, 3);
        // Enable the Timer peripheral




        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//
        TimerDisable(TIMER0_BASE, TIMER_BOTH);
        TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER0_BASE, TIMER_BOTH, SysCtlClockGet());
        TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        TimerIntRegister(TIMER0_BASE,TIMER_BOTH, Timer0IntHandler);
        IntEnable(INT_TIMER0A);

        TimerEnable(TIMER0_BASE, TIMER_BOTH);
        TimerControlTrigger(TIMER0_BASE, TIMER_A, true);

}
