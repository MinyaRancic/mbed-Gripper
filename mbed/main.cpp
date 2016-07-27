#include "mbed.h"
#include "Motor.h"
#include "LS7366.h"
#include "PID.h"
//#include "rtos.h"
#include "MODSERIAL.h"
//#include "string.h"
#include <ctype.h>
#include <string>
#include <algorithm>

#define OPEN 0
#define CLOSED 62

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
AnalogIn currentMonitoring(p17);



/**
* Declarations for all needed hardware
*/
volatile float kU = .00075f;
volatile float tU = .27;
volatile float kP = kU*.6;
volatile float kI = (2*kP)/tU;
volatile float kD = (kP * tU) / 8;

SPI spi(p5, p6, p7); //p11, p12, and p13 go to a serial driver
MODSERIAL pc(USBTX, USBRX);
Motor motor(p23, p25, p24);
LS7366 encoder(spi, p8);
PID controller(kP, (2*kP)/tU, (kP*tU)/8, 5); // tested values: Kp = .007, Ki = .001, Kd = .00007, t = 8

//276 encoder counts per 58 degrees
const float countsPerDegree = 276/58.0;

/**
* Pins used for overiding from automatic control to manual control
* 2 pins (p28 and p29) are used for controlling directional movement (note: left may not be left, right may not be right)
*/
DigitalIn left(p29);
DigitalIn right(p28);
DigitalIn overide(p27);


float motorSpeed;
int enc1;
volatile int setpoint;
volatile int numChars = 0;
char buffer[128];
bool data = false;

float getCurrent()
{
    return (currentMonitoring.read() * 3.3)/.525;
}

/**
 * This method is a callback function that runs whenever a newline character
 * is detected in the Rx buffer.
 * It transfers all data from the Rx Buffer to the 128 char array buffer
 */
void rxInterupt(MODSERIAL_IRQ_INFO *q)
{
    led4 = 1;
    MODSERIAL *tempSerial = q->serial;
    tempSerial->move(buffer, 128);
    data = true;
    led4 = 0;
}

/**
 * The processData method takes the data obtained from the buffer and processes it
 * it only runs whenever a command is sent, and will most likely crash the program if the command is formatted improperly
 */
void processData()
{
    led1 = 0;
    led2  = 0;
    led3 = 0;
    led4 = 0;
    /* command has 2 possible values
     * command == 0 - set a variable
     * command == 1 - get a variable
     */
    int command;
    
    /* property has 5 states and chooses which variable an action is being applied to
     * property == 0 - setpoint
     * property == 1 - kP
     * property == 2 - kI
     * property == 3 - kD
     * property == 4 - error *Note: you can only get the error
     */
    int property;
    
    //value is only used for set commands, and set's the value of a variable
    float value;
    sscanf(buffer, "%d,%d,%f", &command, &property, &value);  //command format is two integer values, followed by a float
    led4 = 0;
    if(command == 0)
    {
        led1 = 1;
        if(property == 0)
            setpoint = value * countsPerDegree;
        else if(property == 1)
            kP = value;
        else if(property == 2)
            kI = value;
        else if(property == 3)
            kD = value;         
    }
    else if(command == 1)
    {
        led2 = 1;
        if(property == 0)
            pc.printf("%f\n", encoder.read()/countsPerDegree);
        else if(property == 1)
            pc.printf("%f\n", kP);
        else if(property == 2)
            pc.printf("%f\n", kI);
        else if(property == 3)
            pc.printf("%f\n", kD);
        else if(property == 4)
            pc.printf("%f\n", controller.getError());
        else if(property == 5)
            pc.printf("%f\n", getCurrent());
        else if(property == 6)
            pc.printf("%f\n", motorSpeed);
    }
    else
    {
        pc.printf("%f,%f,%f\n", command, property, value);
        led4 = 3;
    }
    
    controller.setTunings(kP, kI, kD);
}

int main() {
    //setpoint = 55 * countsPerDegree;
    
    controller.setInputLimits(-.5, 1);
    
    int enc1 ;
    controller.setSetpoint(setpoint);
    pc.format(8,SerialBase::Even,1);

    
    pc.attach(&rxInterupt, MODSERIAL::RxAutoDetect);
    pc.autoDetectChar('\n');
    


    while(1) 
    {   
        
        if(data)
        {
            processData();
            data = false;
        }
        enc1 = encoder.read();
        controller.setSetpoint(setpoint);
            if(overide) //statment for manual control - I should probably make tihs a method
            {
                if(left)
                    motor.speed(-.5);
                else if(right)
                    motor.speed(.5);
                else
                    motor.speed(0);
            }
            else //statement for auto control - also should probably be put in a method
            {
    
                motorSpeed = controller.calculateOutput(enc1);
                motor.speed(motorSpeed);
            }
            if (controller.isFinished())
            {
                //pc.printf("Angle: %f\r", encoder.read()/countsPerDegree);
                //wait_ms(5);
            }
            else
            {
               // pc.printf("Encoder count: %d  \t\t  %d\n", enc1, controller.isFinished());
                //wait_ms(5);
            }
        
    }
}


