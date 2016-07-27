/**
* So I don't realy know how header files work
* so this will mostly be based off of motor.h's header file
*/

#include "mbed.h"

#ifndef PID_H
#define PID_H
class PID
{
    public:
        /**
        * constructor
        * @param kP - proportional tuning coefficient
        * @param kI - integral tuning coefficient
        * @param kD - Derivative tuning coefficient
        * @param tolerance - defines an acceptable tolerance off of the setPoint
        */
        PID(float kP, float kI, float kD, float tolerance);
        
        /**
        * setter methods
        */
        
        void setTunings(float kP, float kI, float kD);
        void setTolerance(float tolerance);
        void setInputLimits(float min, float max);  
        void setSetpoint(float input);
        void reset();
          
        /**
        * getter methods
        */
        
        float getkP();
        float getkI();
        float getkD();
        float getError();
        
        /**
        * Calculation methods
        */
        
        void calculateIntegral();
        float calculateDerivative();
        float calculateOutput(float input);
        
        bool isFinished();
    private:
        float _kP;
        float _kI;
        float _kD;
        float _tolerance;
        float _minInput, _maxInput;
        float _error, _lastError;
        float _dt;
        float _setpoint;
        float _totalIntegral;
        Timer _timer;
        Timer _steadyTimer;
        bool _steady;
};
#endif