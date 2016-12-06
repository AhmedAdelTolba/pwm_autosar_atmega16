#include "PWM_cfg.h"
#include "Pwm_Prototypes.h"
#include <avr/io.h>
#include "util.h"
#include <avr/interrupt.h>



int main()
    {

    // Global enable interrupts
    sei();

    DDRD = 0xff;
    DDRB = 0xff;
    PORTB = 0;

    Pwm_Init(0);

    //  Pwm_Deinit();

//    Pwm_SetDutyCycle(LogicalPwmChannel1, 0);
//    Pwm_SetDutyCycle(LogicalPwmChannel2, 0x4001);
//    Pwm_SetDutyCycle(LogicalPwmChannel1, 0x7005);
//    Pwm_SetDutyCycle(LogicalPwmChannel2, 0);
//    Pwm_SetDutyCycle(LogicalPwmChannel2, 0x2005);
//    Pwm_SetPeriodAndDuty(LogicalPwmChannel2, Period3, 0x4001);
//    Pwm_SetPeriodAndDuty(LogicalPwmChannel1, Period1, 0x4001);
//    Pwm_SetOutputToIdle(LogicalPwmChannel1);
//    Pwm_SetOutputToIdle(LogicalPwmChannel2);
//    Pwm_SetPeriodAndDuty(LogicalPwmChannel2, Period3, 0x4001);
//    Pwm_SetPeriodAndDuty(LogicalPwmChannel1, Period1, 0x4001);

  Pwm_EnableNotification(LogicalPwmChannel1,PWM_RISING_EDGE);


    while (1)
	{

	}
    return 0;
    }

//#include <avr/io.h>
//#include <avr/interrupt.h>
//
//// Timer1 overflow interrupt service routine
//ISR(TIMER1_COMPA_vect)
//    {
//    PORTB ^= 1; // invert PORTB.0
//    }
//
//int main(void)
//    {
//
//    // Port B initialization
//    // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=Out Bit1=In Bit0=Out
//    DDRD = 0xff;
//    DDRB = 0xff;
//    // Timer/Counter 1 initialization
//    // Clock source: System Clock
//    // Clock value: 2000,000 kHz
//    // Mode: Fast PWM top=ICR1
//    // OC1A output: Disconnected
//    // OC1B output: Non-Inverted PWM
//    // Noise Canceler: Off
//    // Input Capture on Falling Edge
//    // Timer Period: 1,0245 ms
//    // Output Pulse(s):
//    // OC1B Period: 1,0245 ms Width: 0,12806 ms
//    // Timer1 Overflow Interrupt: On
//    // Input Capture Interrupt: Off
//    // Compare A Match Interrupt: Off
//    // Compare B Match Interrupt: Off
//    TCCR1A = (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0) | (1 << WGM11) | (0 << WGM10);
//    TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);
//
//    ICR1 = 0x4000;
//    OCR1A = 0x2050;
//    OCR1B = 0x2080;
//
//    // Timer/Counter 1 Interrupt(s) initialization
//    TIMSK =  (1 << OCIE1A) ;
//
//    // Global enable interrupts
//    sei();
//
//    while (1)
//	{
//	// Place your code here
//
//	}
//    }
