/* Pixy2 SPI control
 * Hamish Sams 2020 :D
 * ShARC 2020
 */
#include "mbed.h"
using namespace std;

SPI spi(D11, D12, D13); // mosi, miso, sclk
DigitalOut cs(D10);

uint16_t littleEndianJoinUINT8_t(uint8_t MSB,uint8_t LSB){
    uint16_t answer = (((uint16_t)MSB)<<8) | LSB;
    return answer;
}

uint32_t littleEndianJoinUINT16_t(uint16_t MSB,uint16_t LSB){
    uint32_t answer = (((uint16_t)MSB)<<16) | LSB;
    return answer;
}

struct Vector {
  uint8_t m_x0;
  uint8_t m_y0;
  uint8_t m_x1;
  uint8_t m_y1;
  uint8_t m_index;
  uint8_t m_flags;
} ;

struct IntersectionLine{
    uint8_t m_index;
    int16_t m_angle;
};

struct Intersection{
    uint8_t m_x;
    uint8_t m_y;
    uint8_t m_n;
    IntersectionLine m_intLines[255];
};

struct Barcode{
    uint8_t m_x;
    uint8_t m_y;
    uint8_t m_flags;
    uint8_t m_code;
};

class Pixy2{
    public:
    uint16_t hardware;
    uint8_t firmwareMajor;
    uint8_t firmwareMinor;
    uint16_t firmwareBuild;
    char firmwareType[20];
    uint16_t frameHeight;
    uint16_t frameWidth;
    int32_t resultCode;
    Vector vectors[255];
    Intersection intersections[255];
    Barcode barcodes[255];
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
        if(length==255)
            return -1;
        uint8_t * data = (uint8_t *) malloc((length+1)*sizeof(uint8_t ));

        for(int i=0;i<length;i++)
            data[i]=spi.write(0x00);
        SPIDisable();

        hardware = littleEndianJoinUINT8_t(data[1],data[0]);
        firmwareMajor = data[2];
        firmwareMinor = data[3];
        firmwareBuild = littleEndianJoinUINT8_t(data[5],data[4]);

        for(int i=6;i<length;i++)
            firmwareType[i-6]=data[i];
        firmwareType[length-6]='\0';
        return 0;
    }

    int8_t getResolution(){
        SPIEnable();
        checkSumSync();
        packetTypeAndLength(12,1);
        spi.write(0);

        uint8_t length = syncAndGetLengthIgnoreChecksum(13);
        if(length==255)
            return -1;
        uint8_t * data = (uint8_t *) malloc((length+1)*sizeof(uint8_t ));

        for(int i=0;i<length;i++)
            data[i]=spi.write(0x00);
        SPIDisable();

        frameWidth = littleEndianJoinUINT8_t(data[1],data[0]);
        frameHeight = littleEndianJoinUINT8_t(data[3],data[2]);
        return 0;
    }

    int8_t setCameraBrightness(uint8_t brightness){
        SPIEnable();
        checkSumSync();
        packetTypeAndLength(16,1);
        spi.write(brightness);

        uint8_t length = syncAndGetLengthIgnoreChecksum(1);
        if(length==255)
            return -1;
        uint8_t * data = (uint8_t *) malloc((length+1)*sizeof(uint8_t ));

        for(int i=0;i<length;i++)
            data[i]=spi.write(0x00);
        SPIDisable();

        resultCode = littleEndianJoinUINT16_t(littleEndianJoinUINT8_t(data[3],data[2]),littleEndianJoinUINT8_t(data[1],data[0]));
        return 0;
    }

    int8_t setLED(uint8_t r,uint8_t g,uint8_t b){
        SPIEnable();
        checkSumSync();
        packetTypeAndLength(20,3);
        spi.write(r);
        spi.write(g);
        spi.write(b);

        uint8_t length = syncAndGetLengthIgnoreChecksum(1);
        if(length==255)
            return -1;
        uint8_t * data = (uint8_t *) malloc((length+1)*sizeof(uint8_t ));

        for(int i=0;i<length;i++)
            data[i]=spi.write(0x00);
        SPIDisable();

        resultCode = littleEndianJoinUINT16_t(littleEndianJoinUINT8_t(data[3],data[2]),littleEndianJoinUINT8_t(data[1],data[0]));
        return 0;
    }

    int8_t setLamp(uint8_t upper,uint8_t lower){
        SPIEnable();
        checkSumSync();
        packetTypeAndLength(22,2);
        spi.write(upper);
        spi.write(lower);

        uint8_t length = syncAndGetLengthIgnoreChecksum(1);
        if(length==255)
            return -1;
        uint8_t * data = (uint8_t *) malloc((length+1)*sizeof(uint8_t ));

        for(int i=0;i<length;i++)
            data[i]=spi.write(0x00);
        SPIDisable();

        resultCode = littleEndianJoinUINT16_t(littleEndianJoinUINT8_t(data[3],data[2]),littleEndianJoinUINT8_t(data[1],data[0]));
        return 0;
    }

    int8_t getFPS(){
        SPIEnable();
        checkSumSync();
        packetTypeAndLength(24,0);

        uint8_t length = syncAndGetLengthIgnoreChecksum(1);
        if(length==255)
            return -1;
        uint8_t * data = (uint8_t *) malloc((length+1)*sizeof(uint8_t ));

        for(int i=0;i<length;i++)
            data[i]=spi.write(0x00);
        SPIDisable();

        resultCode = littleEndianJoinUINT16_t(littleEndianJoinUINT8_t(data[3],data[2]),littleEndianJoinUINT8_t(data[1],data[0]));
        return 0;
    }

    int8_t getMainFeatures(uint8_t features=1,uint8_t wait=0){ //features and wait not implemented correctly
        SPIEnable();
        checkSumSync();
        packetTypeAndLength(48,2);
        spi.write(features);
        spi.write(7);


        uint8_t length = syncAndGetLengthIgnoreChecksum(49);
        if(length==255)
            return -1;
        uint8_t * data = (uint8_t *) malloc((length+1)*sizeof(uint8_t ));

        for(int i=0;i<length;i++)
            data[i]=spi.write(0x00);
        SPIDisable();

        for(int i=0;i<length;i++)
            printf("Data %d: %d\n",i,data[i]);
        
        int startingBit=0;
        int numberOfIntersections=0,numberOfVectors=0,numberOfBarcodes=0;

        while((data[startingBit]!=1)||(data[startingBit]!=2)||(data[startingBit]!=4)){
            if(data[startingBit]==1){//if vector
                vectors[numberOfVectors].m_x0=data[startingBit+2];
                vectors[numberOfVectors].m_y0=data[startingBit+3];
                vectors[numberOfVectors].m_x1=data[startingBit+4];
                vectors[numberOfVectors].m_y1=data[startingBit+5];
                vectors[numberOfVectors].m_index=data[startingBit+6];
                vectors[numberOfVectors].m_flags=data[startingBit+7];
                numberOfVectors++;
            }
            if(data[startingBit]==2){//if intersection
                intersections[numberOfIntersections].m_x=data[startingBit+2];
                intersections[numberOfIntersections].m_y=data[startingBit+3];
                intersections[numberOfIntersections].m_n=data[startingBit+4];
                int intersectionLineCount=0;
                for(int i=startingBit+5;i<2*data[startingBit+4];i+=2){
                    intersections[numberOfIntersections].m_intLines[intersectionLineCount].m_index = data[i];
                    intersections[numberOfIntersections].m_intLines[intersectionLineCount].m_angle = data[i+1];
                    intersectionLineCount++;
                }
                numberOfIntersections++;
                
            }
            if(data[startingBit]==4){//if barcode
                barcodes[numberOfBarcodes].m_x=data[startingBit+2];
                barcodes[numberOfBarcodes].m_y=data[startingBit+3];
                barcodes[numberOfBarcodes].m_flags=data[startingBit+4];
                barcodes[numberOfBarcodes].m_code=data[startingBit+4];
                numberOfBarcodes++;
            }
            startingBit+=data[startingBit+1]+2;
        }
        

        return 0; //returing the correct thing?

    }

    
    
    
    int8_t getBlocks(uint8_t sigmap=255, uint8_t maxBlocks=255){
        SPIEnable();
        checkSumSync();
        packetTypeAndLength(32,2);
        spi.write(sigmap);
        spi.write(maxBlocks);

        uint8_t length = syncAndGetLengthIgnoreChecksum(33);
        if(length==255)
            return -1;
        uint8_t * data = (uint8_t *) malloc((length+1)*sizeof(uint8_t ));

        for(int i=0;i<length;i++)
            data[i]=spi.write(0x00);
        SPIDisable();

        for(int i=0;i<length;i++)
            printf("Byte %d: %d\n",i,data[i]);
        return 0;
        }//todo

};




 
int main() {
    printf("Test\n");
    Pixy2 pixy;
    pixy.init();
    int error = pixy.setLamp(255,255);
    printf("Error Code %d\n",error);
    return 0;
}