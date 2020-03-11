#include "mbed.h"
#include "SBL1360.h"

Timer t;

int main() {

    SBL1360 sbl;
    sbl.speed(100);

    // start timer to run motor for 3s
    t.start();

    while(1){
        // must run update() at at least 2Hz
        sbl.update();

        // set motor speed to zero after 3s
        if (t.read() > 3){
            sbl.speed(0);
            while(1){;}
        }
    }
    
}