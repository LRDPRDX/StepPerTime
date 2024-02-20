#include "StepPerTime.h"

namespace steppo
{
    Stepper::Stepper( uint32_t speed, uint8_t accel ) :
        m_speedMax( speed ),
        m_acceleration( accel )
    { }

    bool Stepper::onInterrupt()
    {
        if( not (this->*m_state.action)() )
            return false;

        m_stepFraction += m_speedCurrent;

        if( stepOverflow() )
        {
            m_stepsCurrent++;
            m_newStep = true;
        }
        else
            m_newStep = false;

        return true;
    }

    bool Stepper::start( uint32_t n )
    {
        if( stateCurrent() != EState_t::eIdle )
            return false;

        reset();
        m_stepsRequired = n;
        m_midPoint = n / 2;
        setState( EState_t::eAccelerate );

        return true;
    }

    bool Stepper::stop()
    {
        if(stateCurrent() == EState_t::eIdle )
            return false;

        setState( EState_t::eDecelerate );

        return true;
    }

    bool Stepper::idle()
    {
        return false;
    }

    bool Stepper::accelerate()
    {
        m_speedCurrent += m_acceleration;

        if( m_stepsCurrent == m_midPoint )
        {
            setState( EState_t::eDecelerate );
            return true;
        }

        if( m_speedCurrent >= m_speedMax )
        {
            setState( EState_t::eRun );
            m_speedCurrent = m_speedMax;
            calcDecelerationPoint();
        }

        return true;
    }

    bool Stepper::run()
    {
        if( m_stepsCurrent == m_decelerationPoint )
            setState( EState_t::eDecelerate );

        return true;
    }

    bool Stepper::decelerate()
    {
        if( m_speedCurrent < m_acceleration )
            m_speedCurrent = 0;
        else
            m_speedCurrent -= m_acceleration;

        if( m_speedCurrent == 0 )
            setState( EState_t::eIdle );

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
        setState( EState_t::eIdle );

        m_midPoint          = 0;
        m_stepsCurrent      = 0;
        m_stepFraction      = 0;
        m_decelerationPoint = 0;
        m_speedCurrent      = 0;
        m_newStep           = false;
    }

    void Stepper::setState( const EState_t code )
    {
        m_state.code = code;
        switch( code )
        {
            case( EState_t::eIdle ) :
                m_state.action = &Stepper::idle; break;
            case( EState_t::eAccelerate ) :
                m_state.action = &Stepper::accelerate; break;
            case( EState_t::eDecelerate ) :
                m_state.action = &Stepper::decelerate; break;
            case( EState_t::eRun ) :
                m_state.action = &Stepper::run; break;
            default :
                m_state.action = &Stepper::decelerate; break;
        }
    }
}//steppo

