/* Pixy2 SPI control
 * Hamish Sams 2020:D
 */
#include "mbed.h"
using namespace std;

SPI spi(D11, D12, D13); // mosi, miso, sclk
DigitalOut cs(D10);

class Pixy2{
    public:
    char firmwareType[20];
    int8_t init(){
        SPIDisable();
        spi.format(8,3);
        spi.frequency(2000000);
        return 0;
    }
    void SPIEnable(){
        cs=0;
    }
    void SPIDisable(){
        cs=1;
    }
    void checkSumSync(){
        spi.write(0xAE);
        spi.write(0xC1);
    }
    void packetTypeAndLength(uint8_t type,uint8_t length){
        spi.write(type);
        spi.write(length);
    }
    uint8_t syncAndGetLengthIgnoreChecksum(uint8_t packetTypeID){
        uint8_t length=255;
        for(int i=0;i<1000;i++){
            if(spi.write(0x00)==175)
                if(spi.write(0x00)==193)
                    if(spi.write(0x00)==packetTypeID){
                        length = spi.write(0x00);
                        break;
                    }
        }
        spi.write(0x00);
        spi.write(0x00);
        return length;
    }
    int8_t getVersion(){
        SPIEnable();
        checkSumSync();
        packetTypeAndLength(14,0);

        uint8_t length = syncAndGetLengthIgnoreChecksum(15);
        uint8_t * data = (uint8_t *) malloc((length+1)*sizeof(uint8_t ));

        for(int i=0;i<length;i++)
            data[i]=spi.write(0x00);
        SPIDisable();

        for(int i=6;i<length;i++)
            firmwareType[i-6]=data[i];
        firmwareType[length-6]='\0';
        return 0;
    }

};




 
int main() {
    spi.format(8,3);
    spi.frequency(2000000);
    Pixy2 pixy;
    pixy.init();
    pixy.getVersion();
    printf("Response: %s\n", pixy.firmwareType);
    
 
}