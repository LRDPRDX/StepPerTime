#pragma once

#ifndef ARDUINO
#include <cstdint>
#else
#include <Arduino.h>
#endif


namespace steppo
{
    class Stepper
    {
        public :
            Stepper( uint32_t n, uint32_t speed, uint8_t accel );

            enum EState_t
            {
                eIdle,
                eAccelerate,
                eDecelerate,
                eRun,
            };

            bool onInterrupt();
            void start();
            void stop();

            bool        newStep()      const { return m_newStep; };
            uint32_t    speedCurrent() const { return m_speedCurrent; };
            uint32_t    stepsCurrent() const { return m_stepsCurrent; };
            EState_t    currentState() const { return m_state.code; };

        private :
            // Types
            using Action_t = bool (Stepper::*)(void);
            struct State_t
            {
                EState_t code   { EState_t::eIdle };
                Action_t action { &Stepper::idle };
            };

            // States
            bool idle();
            bool accelerate();
            bool decelerate();
            bool run();

            // Aux
            bool stepOverflow();
            void calcDecelerationPoint();
            void reset();
            void setState( const EState_t code );

            // Data
            State_t     m_state;

            uint32_t    m_stepsRequired { 0 };
            uint32_t    m_stepsCurrent { 0 };
            uint32_t    m_stepFraction { 0 };
            uint32_t    m_midPoint { 0 };
            uint32_t    m_decelerationPoint { 0 };

            uint32_t    m_speedMax { 0 };
            uint32_t    m_speedCurrent { 0 };

            uint16_t    m_acceleration { 0 };

            bool        m_newStep { false };
    };
}//steppo

