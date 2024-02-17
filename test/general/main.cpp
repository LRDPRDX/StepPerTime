#include <cassert>

#include "StepPerTime.h"

using namespace steppo;

int main()
{
    constexpr uint32_t speed    = 1000;
    constexpr uint32_t nSteps   = 100;
    constexpr uint8_t  accel    = 5;

    Stepper s( speed, accel );

    assert( s.stateCurrent() == Stepper::EState_t::eIdle );
    assert( s.stepsCurrent() == 0 );
    assert( s.speedCurrent() == 0 );
    assert( s.newStep()      == false );

    s.start( nSteps );
    assert( s.stateCurrent() == Stepper::EState_t::eAccelerate );
    assert( s.speedCurrent() == 0 );

    assert( s.onInterrupt() == true );
    assert( s.speedCurrent() > 0 );

    while( s.onInterrupt() ) { }

    assert( s.stepsCurrent() == nSteps );
    assert( s.stateCurrent() == Stepper::EState_t::eIdle );

    s.start( nSteps );
    assert( s.stateCurrent() == Stepper::EState_t::eAccelerate );

    while( s.onInterrupt() )
    {
        if( s.speedCurrent() == speed )
        {
            assert( s.stateCurrent() == Stepper::EState_t::eRun );

            s.stop();
            assert( s.stateCurrent() == Stepper::EState_t::eDecelerate );
        }
    }

    return 0;
}
