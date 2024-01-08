#include "StepPerTime.h"

steppo::Stepper stepper( 2000, 10000, 200 );

void setup()
{
    Serial.begin( 115200 );

    cli();

    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= _BV( WGM12 ); // CTC mode
    TCCR1B |= _BV( CS12 );  // 1024 prescaler
    TIMSK1 = _BV( OCIE1A ); // Interrupt on compare match
    OCR1A  = 63;            // 992 Hz 

    sei();

    stepper.start();

    Serial.println( "Start!" )
}

void loop()
{
}

ISR( TIMER1_COMPA_vect )
{
    if( not stepper.isr() )
    {
        Serial.println( "Done!" )
        return;
    }

    if( stepper.newStep() )
    {
        /* your pulse code */
        return;
    }
}

