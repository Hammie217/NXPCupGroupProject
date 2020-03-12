#include "mbed.h"
#include "SBL1360.h"

Serial rs232(D14, D15);

#ifdef BLUETOOTH_BUF_LENGTH
Serial bt(PTE22, PTE23); // UART2
#endif

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
            #ifdef BLUETOOTH_BUF_LENGTH
            bt.baud(115200);
            #endif
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
            #ifdef BLUETOOTH_BUF_LENGTH
            if(bt.readable()){
                char charFromBt;
                charFromBt = bt.getc();
                //pc.printf("char incoming: %c\n", charFromBt);
                if(charFromBt == '\r'){
                    int intFromBt = 0;
                    for(int n=0; n<bufIndex; n++){
                        int powerOf10 = 1;
                        for(int m=0; m<bufIndex-n-1; m++){
                            powerOf10 *= 10;
                        }
                        intFromBt += bufFromBt[n]*powerOf10; // convert array to number
                    }
                    bufIndex = 0;
                    this->speed(intFromBt);
                } else if (charFromBt > 47 && charFromBt < 58){
                    if (bufIndex == BLUETOOTH_BUF_LENGTH) bufIndex = 0;
                    bufFromBt[bufIndex++] = charFromBt-48;  // convert to ASCII
                    //pc.printf("valid char not end: %d\nnext buf index: %d\n", charFromBt-48, bufIndex);
                } else {
                    bufIndex = 0;
                }
            }
            #endif
        }
