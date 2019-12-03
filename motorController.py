from periphery import PWM, I2C
import time
class ESC():
    def __init__(self,pin):
        if(pin == 32):
            PWMOut = 0
        elif(pin==33):
            PWMOut = 1
        elif(pin==15):
            PWMOut = 2
        else:
            print("Assumed PWM chip number given")
            PWMOut = pin
        self.controllerNumber = PWMOut
        self.pwm = PWM(PWMOut, 0)
        self.pwm.frequency = 50

    
    def setFrequency(self,frequency):
        self.pwm.frequency = frequency
    
    def setSpeed(self,Duty):
        if((Duty>=0)and(Duty<=100)):
            Duty=(Duty*0.0005)+0.05
            self.pwm.duty_cycle = Duty
        elif(Duty==-1):
            self.pwm.duty_cycle = 0
        else:
            print("Incorrect duty (0 to 100)")
            self.pwm.duty_cycle=0.05

    def start(self):
        self.pwm.enable()

    def stop(self):
        self.setSpeed(-1)
        self.pwm.close()

class Servo():
    def __init__(self,pin=0):
        if(pin == 32):
            PWMOut = 0
        elif(pin==33):
            PWMOut = 1
        elif(pin==15):
            PWMOut = 2
        else:
            print("Assumed PWM chip number given")
            PWMOut = pin
        self.controllerNumber = PWMOut
        self.pwm = PWM(PWMOut, 0)
        self.pwm.frequency = 50

    
    def setFrequency(self,frequency):
        self.pwm.frequency = frequency
    
    def setPosition(self,Rotation):
        if((Rotation>-90)and(Rotation<90)):
            Duty = 0.075+(Rotation*0.05/180)
            self.pwm.duty_cycle = Duty
        elif(Rotation==-100):
            self.pwm.duty_cycle = 0
        else:
            print("Incorrect postion (-90 to 90)")
            self.pwm.duty_cycle=0.05

    def start(self):
        self.pwm.enable()

    def stop(self):
        self.setPosition(-100)
        self.pwm.close()


class I2CPin():
    def __init__(self,pin):
        if(pin==1):
            self.i2c=I2C("/dev/i2c-1")
        elif(pin==2):
            self.i2c=I2C("/dev/i2c-2")
        else:
            print("Invalid I2C pin given (1-2)")

    def sendData(self,message):
        self.i2c.transfer(0x50,message)

    def stop(self):
        self.i2c.close()


def testMotorsAndSteering():
    motor1 = ESC(32)
    motor2 = ESC(33)
    steering = Servo(15)
    motor1.start()
    motor2.start()
    steering.start()


    for i in range(0,90):
        motor1.setSpeed(i)
        motor2.setSpeed(i)
        steering.setPosition((i*2)-90)
        print(i)
        time.sleep(0.1)

    motor1.stop()
    motor2.stop()
    steering.stop()

def testI2CBus():
    board1=I2CPin(1)
    board1.sendData("Hello")
    board1.stop()

testI2CBus()

