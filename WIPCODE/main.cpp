#include "mbed.h"
#include "Pixy2.h"
#include "global_parts.h"
#include "cmath"
#include "SBL1360.h"
#include "PID.h"

#define PI 3.14159265
#define NOLINESSTOP 100 //0-255
#define DEBUG 1 //1 for serial output, 0 for off
#define FUDGEFACTOR 1
#define SERVO_LIMIT 0.9

#ifndef DEBUG
    #define DEBUG 0
#endif

PwmOut servo(D10);
Pixy2 pixy;
Timer t; 
SBL1360 sbl;
//PID defs
double Output;
double Setpoint;
double errors=0;//PID input
double currentPoint=0;
double ratio;//PID output
 
//Specify the links and initial tuning parameters
double Kp=100, Ki=200, Kd=5;
PID myPID(&currentPoint, &ratio, &Setpoint, Kp, Ki, Kd, P_ON_M, DIRECT);

void initServo(){
    servo.period_ms(20);//20ms delay aka 50Hz
    servo.pulsewidth_us(1500); //1500 us high time aka 1.5ms
}

void setServoRatio(double desiredRatio){
    // <-1 desired ratio <1
    desiredRatio *= FUDGEFACTOR;
    if((desiredRatio>SERVO_LIMIT))
        desiredRatio=SERVO_LIMIT;
    else if((desiredRatio<-SERVO_LIMIT))
        desiredRatio=-SERVO_LIMIT;
    uint16_t desiredOnTime = floor(500*desiredRatio)+1500;
    servo.pulsewidth_us(desiredOnTime);
}

int main(void)
{
    // ------------------------------ setup ------------------------------
    if(DEBUG){
        pc.baud(115200);
        pc.printf("Starting...\n");
    }

    pixy.init();
    initServo();

    pixy.setLamp(0,0);
    pixy.changeProg("line_tracking");
    pixy.getResolution();
    currentPoint = (double)pixy.frameWidth/2;
    myPID.SetMode(AUTOMATIC);
    myPID.SetOutputLimits(-1, 1);

    uint8_t blankInputTimer = 0;
    int driveForward =0;

    // ------------------------------ main ------------------------------

    while(1)
    {
        sbl.update();
        
        pixy.line.getMainFeatures();
        if (pixy.line.numVectors){
            //pixy.line.vectors->print();
            //pc.printf("x0: %d, y0: %d, x1: %d, y1: %d\n",pixy.line.vectors->m_x0,pixy.line.vectors->m_y0,pixy.line.vectors->m_x1,pixy.line.vectors->m_y1);
            //errors = (double)((double)pixy.line.vectors->m_x1-centerX);
            //ratio = ((double)errors)/centerX;
            Setpoint=(double)pixy.line.vectors->m_x1;
            //if(DEBUG) pc.printf("centerX %f\nerror: %f\nratio: %f\n",centerX,errors,ratio); 
            blankInputTimer=NOLINESSTOP;
            driveForward=1;
            if(DEBUG)
                pc.printf("Ratio: %f\n",ratio); 

        }
        else {
            if(blankInputTimer>0){
                blankInputTimer--;
                //ratio=0; //keep ratio the same
                driveForward=1;
                myPID.Compute();
                if(DEBUG)
                    pc.printf("Ratio: %f\n",ratio); 

            }
            else{
                driveForward=0;
            if(DEBUG)
                pc.printf("No line for a long time\n"); 
            }
        }
        if(driveForward==1){
            myPID.Compute();
            setServoRatio(-ratio);
        }
    };
}