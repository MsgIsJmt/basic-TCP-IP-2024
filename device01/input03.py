import RPi.GPIO as GPIO
import time

r_led = 21
piezoPin = 13
melody = [130, 147, 165, 175, 196, 220, 247, 262]

GPIO.setmode(GPIO.BCM)
GPIO.setup(r_led, GPIO.OUT)
GPIO.setup(piezoPin, GPIO.OUT)

Buzz = GPIO.PWM(piezoPin, 440)

try:
	while (True):
		a = input()
		if a == 'o':
			GPIO.output(r_led,False)
			time.sleep(1)

		elif a == 'x':
			GPIO.output(r_led,True)
			time.sleep(1)

		elif a in {'1', '2', '3', '4', '5', '6', '7', '8'}:
			a = int(a)
			Buzz.start(50)
			Buzz.ChangeFrequency(melody[a-1])

		elif a in '0':
			Buzz.stop()
			time.sleep(1)

except KeyboardInterrupt: 
	GPIO.cleanup()
