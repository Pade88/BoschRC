import RPi.GPIO as GPIO
import time


def sendMove(speed, angle):
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(16, GPIO.OUT) #DIRECTIE

    if speed > 0:
        GPIO.output(16, 1) #FORWARD
    else:
        GPIO.output(16, 0) # BACKWARD
    
    speed=abs(speed)
    if angle < 15 or angle > 95:
        return
    else:
        GPIO.setup(25, GPIO.OUT)
        servo = GPIO.PWM(25, 1)
        servo.start(angle)
        time.sleep(1)
        servo.stop()
        
    if speed < 15 or speed > 75:
        return
    else:
        GPIO.setup(12, GPIO.OUT)
        engine = GPIO.PWM(12, 1)
        engine.start(speed)
        time.sleep(1)
        engine.stop()  
        