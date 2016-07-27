/**
* The PID library was confusing
* so i'm just going to write my own
* hopefully it'll work and i'll be able to understand it better
*/

#include "PID.h"

PID::PID(float kP, float kI, float kD, float tolerance) : _kP(kP), _kI(kI), _kD(kD), _tolerance(tolerance)
{
        _timer.start();
        _totalIntegral = 0;
}

void PID::setTunings(float kP, float kI, float kD)
{
    _kP = kP;
    _kD = kD;
    _kI = kI;
}

void PID::setTolerance(float tolerance)
{
    _tolerance = tolerance;
}

void PID::setInputLimits(float min, float max)
{
    _minInput = min;
    _maxInput = max;    
}

void PID::setSetpoint(float input)
{
    if(input == 0)
        _setpoint = .001;
    else
        _setpoint = input;
}
float PID::getkP()
{
    return _kP;
}

float PID::getkI()
{  
    return _kI;
}

float PID::getkD()
{
    return _kD;
}

float PID::getError()
{
    return _error;
}

void PID::calculateIntegral()
{
    _totalIntegral += ((_error) * _dt) / _setpoint;
}
float PID::calculateDerivative()
{
    float derivative;
    derivative = ((_error - _lastError)/_dt) / _setpoint;
    return _kD * derivative;
}

float PID::calculateOutput(float input)
{
    _timer.stop();
    _dt = _timer.read();
    _timer.reset();
    _timer.start();    
    if(input >= (_setpoint - _tolerance) && input <= (_setpoint + _tolerance))
    {
        _steady = true;
        _steadyTimer.start();
        return 0;
    }
    if(_steady)
    {
        _steady = false;
        _steadyTimer.stop();
        _steadyTimer.reset();
    }

    _error = _setpoint - input;
    float dError = this->calculateDerivative();
    
    this->calculateIntegral();
    float iError = _kI* _totalIntegral;
    
    float output = _error / _setpoint;
    output *= this->getkP();
    output += dError;
    output += iError;
    
   // printf("integral Eror %f \n", iError);
    if(output > _maxInput)
        output = _maxInput;
    if(output < _minInput)
        output = _minInput;
    if(output < .5 && output > 0 )
        output = .5;
   /* else if(output < .4 && output > 0 && input > 150)
        output = .4;*/
    else if(output > -.2 && output < 0)
        output = -.2;
        
    _lastError = _error;
    return output;
    
}

void PID::reset()
{
    _totalIntegral = 0;
}
bool PID::isFinished()
{
    //printf("%f\n", _steadyTimer.read());
    return _steadyTimer.read() >= 1;
}