#include "StepPerTime.h"

steppo::Stepper stepper( 10000, 200 );

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

    stepper.start( 1000 );

    Serial.println( "Start!" );
}

void loop()
{
}

ISR( TIMER1_COMPA_vect )
{
    if( not stepper.onInterrupt() )
    {
        Serial.println( "Done!" );
        TIMSK1 = 0;
        return;
    }

    if( stepper.newStep() )
    {
        /* your pulse code */
        return;
    }
}

