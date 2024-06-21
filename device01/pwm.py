#피에조
import RPi.GPIO as GPIO
import time

piezoPin = 13
melody = [130, 147, 165, 175, 196, 220, 247  ,262]
melody2 = [165, 147, 130, 147, 165, 165, 165, 147, 147, 147, 165, 196, 196, 165, 147, 130, 147, 165, 165, 165, 147, 147, 165, 147, 130]

GPIO.setmode(GPIO.BCM)
GPIO.setup(piezoPin, GPIO.OUT)

#아날로그 출력을 위한 객체생성(440Hz 출력)
Buzz = GPIO.PWM(piezoPin, 440)

try:
	while True:
		Buzz.start(50) #duty cycle : 50
		for i in range(0, len(melody2)):
			Buzz.ChangeFrequency(melody2[i])
			time.sleep(0.3)
		Buzz.stop()
		time.sleep(1)

except KeyboardInterrupt:
	GPIO.cleanup()
