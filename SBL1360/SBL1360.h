#ifndef SBL1360_H
#define SBL1360_H

#define BLUETOOTH_BUF_LENGTH 3

class SBL1360{

        int speed_sp = 0;
        #ifdef BLUETOOTH_BUF_LENGTH
        char bufFromBt[BLUETOOTH_BUF_LENGTH];
        int bufIndex = 0;
        #endif

    public:
        SBL1360();

        void speed(int sp);

        void update(void);
};
#endif
