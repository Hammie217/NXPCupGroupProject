from periphery import PWM

class PWMMotor():
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
        if((Duty>0)and(Duty<100)):
            Duty=(Duty*0.0005)+0.05
            self.pwm.duty_cycle = Duty
        elif(Duty==-1):
            self.pwm.duty_cycle = 0
        else:
            print("Incorrect duty (0-100)")
            self.pwm.duty_cycle=0.05

    def start(self):
        self.pwm.enable()

    def stop(self):
        self.setDuty(-1)
        self.pwm.close()


motor1 = PWMMotor(32)
motor1.setSpeed100)
motor1.start()
motor1.stop()


