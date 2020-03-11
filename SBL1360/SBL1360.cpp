#include "mbed.h"
#include "SBL1360.h"

Serial rs232(D14, D15);

Ticker sbl_wdog_serve;
volatile bool sbl_wdog_flag;
void sbl_wdog_serve_isr(void){
    sbl_wdog_flag = 1;
}
        
SBL1360::SBL1360(){
            // initialise SBL1360
            rs232.baud(115200);
            // disable command echo from sbl
            rs232.printf("^ECHOF 1\r");
            // set speed to zero
            rs232.printf("!g 0\r");
            // setup timer to service sbl wdog
            sbl_wdog_flag = 0;
            sbl_wdog_serve.attach(&sbl_wdog_serve_isr, 0.4);
        }
        


void SBL1360::speed(int sp){
            speed_sp = sp;
            rs232.printf("!g %d\r", sp);
        }

void SBL1360::update(void){
            if (sbl_wdog_flag){
                sbl_wdog_flag = 0;
                rs232.printf("!g %d\r", speed_sp);
            }
        }
