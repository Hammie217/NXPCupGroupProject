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
    
    def setFrequency(self,frequency):
        self.pwm.frequency = 50
    
    def setDuty(self,Duty):
        self.pwm.duty_cycle = Duty

    def start(self):
        self.pwm.enable()

    def stop(self):
        self.pwm.close()


motor1 = PWMMotor(32)
motor1.setFrequency(50)
motor1.setDuty(0.10)

motor1.start()
motor1.stop()


