#include "StepPerTime.h"

namespace steppo
{
    Stepper::Stepper( uint32_t n, uint32_t speed, uint8_t accel ) :
        m_stepsRequired( n ),
        m_speedMax( speed ),
        m_acceleration( accel )
    {
        m_midPoint = n >> 1;
    }

    bool Stepper::isr()
    {
        m_stepFraction += m_speedCurrent;

        if( stepOverflow() )
        {
            m_stepsCurrent++;
            m_newStep = true;
        }
        else
            m_newStep = false;

        return (this->*m_state)();
    }

    void Stepper::start()
    {
        m_state = &Stepper::accelerate;
    }

    bool Stepper::stop()
    {
        reset();
        return false;
    }

    bool Stepper::accelerate()
    {
        m_speedCurrent += m_acceleration;

        if( m_stepsCurrent == m_midPoint )
        {
            m_state = &Stepper::decelerate;
            return true;
        }

        if( m_speedCurrent >= m_speedMax )
        {
            m_state = &Stepper::run;
            m_speedCurrent = m_speedMax;
            calcDecelerationPoint();
        }

        return true;
    }

    bool Stepper::run()
    {
        if( m_stepsCurrent == m_decelerationPoint )
            m_state = &Stepper::decelerate;

        return true;
    }

    bool Stepper::decelerate()
    {
        if( m_speedCurrent < m_acceleration )
            m_speedCurrent = 0;
        else
            m_speedCurrent -= m_acceleration;

        if( //(m_stepsCurrent == m_stepsRequired) or
            (m_speedCurrent == 0) )
            m_state = &Stepper::stop;

        return true;
    }

    bool Stepper::stepOverflow()
    {
        if( (m_stepFraction & 0xffff0000U) == 0 )
            return false;

        m_stepFraction &= 0x0000ffffU;
        return true;
    }

    void Stepper::calcDecelerationPoint()
    {
        m_decelerationPoint = m_stepsRequired - m_stepsCurrent;
    }

    void Stepper::reset()
    {
        m_state             = &Stepper::stop;
        m_stepsCurrent      = 0;
        m_stepFraction      = 0;
        m_decelerationPoint = 0;
        m_speedCurrent      = 0;
        m_newStep           = false;
    }
}//steppo

