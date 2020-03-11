#include "mbed.h"
#include "Pixy2.h"
#include "global_parts.h"
#include "cmath"

#define PI 3.14159265
#define NOLINESSTOP 100 //0-255
#define DEBUG 1 //1 for serial output, 0 for off

#ifndef DEBUG
    #define DEBUG 0
#endif

Pixy2 pixy;
Timer t; 

int main(void)
{
    // ------------------------------ setup ------------------------------
    if(DEBUG){
        pc.baud(115200);
        pc.printf("Starting...\n");
    }
        

    pixy.init();

    pixy.setLamp(255,255);
    pixy.changeProg("line_tracking");
    pixy.getResolution();
    const int8_t centerX = (int8_t)pixy.frameWidth/2;
    int8_t error;
    double ratio;
    uint8_t blankInputTimer = 0;
    // ------------------------------ main ------------------------------

    while(1)
    {
        pixy.line.getMainFeatures();
        if (pixy.line.numVectors){
            //pixy.line.vectors->print();
            //pc.printf("x0: %d, y0: %d, x1: %d, y1: %d\n",pixy.line.vectors->m_x0,pixy.line.vectors->m_y0,pixy.line.vectors->m_x1,pixy.line.vectors->m_y1);
            error = pixy.line.vectors->m_x1-centerX;
            ratio = ((double)error)/centerX;
            blankInputTimer=NOLINESSTOP;
            if(DEBUG)
                pc.printf("Ratio: %f\n",ratio); 

        }
        else {
            if(blankInputTimer>0){
                blankInputTimer--;
                ratio=0;
                if(DEBUG)
                    pc.printf("Ratio: %f\n",ratio); 

            }
            else{
            if(DEBUG)
                pc.printf("No line for a long time\n"); 
            }
        }
    };
}