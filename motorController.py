from periphery import PWM

class PWMMotor():
    def __init__(pin,self):
        if(pin == 32):
            PWMOut = 0
        elif(pin==33):
            PWMOut = 1
        elif(pin==15):
            PWMOut = 2
        else:
            print("Assumed PWM chip number given")
            PWMOut = pin
        self.controllerNumber=PWMOut
        self.pwm = PWM(PWMOut, 0)
    
    def setFrequency(freq,self):
        self.pwm.frequency = 50
    
    def setDuty(Duty,self):
        self.pwm.duty_cycle = Duty

    def start(self):
        self.pwm.enable()

    def stop(self):
        self.pwm.close()

# Open PWM chip 0, channel 10

motor1 = PWMMotor(33)
motor1.setFrequency(50)
# Set frequency to 1 kHz
motor1.setDuty(0.05)

motor1.start()
while 1:
    pass
# Stop
motor1.stop()


