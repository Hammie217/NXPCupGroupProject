from periphery import PWM, I2C, Serial
import time
import select
import sys
import argparse

class SBL():
    def __init__(self):
        print('Initialising SBL...')
        self.uart3 = Serial('/dev/ttymxc2', 115200)
        self.uart3.write(b'^ECHOF 1\r')
        self.uart3.write(b'^HSM 1 0\r')

    def getSpeed(self):
        print('Requesting speed...')
        self.uart3.write(b'?BS 1\r')
        buf =self.uart3.read(128, 0.1)
        print(buf)

    def setSpeed(self, speed):
        #print('Setting speed...')
        print(speed)
        self.uart3.write(b'!g ')
        self.uart3.write(speed)
        self.uart3.write(b'/r')

parser = argparse.ArgumentParser()
parser.add_argument("time", type=int)
args = parser.parse_args()

print('Initialising SBL...')
uart3 = Serial("/dev/ttymxc2", 115200)
uart3.write(b"!g 0\r")
for n in range(args.time*5):
    uart3.write(b"!g 75\r")
    time.sleep(0.2)
uart3.write(b"!g 0\r")
print("Closing sbl port...")
uart3.close()
exit()
