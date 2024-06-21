#pir
import RPi.GPIO as GPIO
import time

pirPin = 24

GPIO.setmode(GPIO.BCM)
GPIO.setup(pirPin, GPIO.IN)

try:
	while True:
		if GPIO.input(pirPin) == True:
			print("Detected")
			time.sleep(0.5)

except KeyboardInterrupt:
	GPIO.cleanup()
