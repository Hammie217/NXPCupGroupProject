//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//

#include "mbed.h"
#include "Pixy2.h"
#include "global_parts.h"

Pixy2 pixy;
Timer t; 

int main(void)
{
    // ------------------------------ setup ------------------------------
    pc.baud(115200);
    pc.printf("Starting...\n");

    pixy.init();

    // ------------------------------ main ------------------------------

    while(1)
    {
        int8_t i;

        // ---------- line section ----------
        t.reset();
        t.start();
        pc.printf("Program change (line -> ccc): %d\n", pixy.changeProg("line"));
        t.stop();
        pc.printf("...time taken: %f\n", t.read());
        for (int n = 0; n<256; n++){
            char buf[128];
            
            pixy.line.getMainFeatures();

            if (pixy.line.numVectors)
                pixy.line.vectors->print();
            
            if (pixy.line.numIntersections)
                pixy.line.intersections->print();

            if (pixy.line.barcodes)
                pixy.line.barcodes->print();
        }

        // ---------- object section ----------
        t.reset();
        t.start();
        pc.printf("Program change (ccc->line): %d\n", pixy.changeProg("ccc"));
        t.stop();
        pc.printf("...time taken: %f\n", t.read());
        for (int n = 0; n<256; n++){
            pixy.ccc.getBlocks();
            
            // If there are detect blocks, print them!
            if (pixy.ccc.numBlocks)
            {
                pc.printf("Detected %d", pixy.ccc.numBlocks);
                for (i=0; i<pixy.ccc.numBlocks; i++)
                {
                    pc.printf(" block %d: ", i);
                    pixy.ccc.blocks[i].print();
                }
            } 

        }
    };
}