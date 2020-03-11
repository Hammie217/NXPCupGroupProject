#include "mbed.h"

Serial pc(USBTX, USBRX); // tx, rx

Timer delayMicrosecondsTimer;

void delayMicroseconds(unsigned int delay){
    float delay_s = delay/1000000;
    delayMicrosecondsTimer.start();
    while(delayMicrosecondsTimer.read()<delay_s){;}
    delayMicrosecondsTimer.stop();
}