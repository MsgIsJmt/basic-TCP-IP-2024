import RPi.GPIO as GPIO
import time

led = 21

#GPIO를 BCM모드로 설정
GPIO.setmode (GPIO.BCM)
#GPIO핀 설정(입력/출력)
GPIO.setup(led, GPIO.OUT)

try:
	while True:
		GPIO.output(led, False)

except KeyboardInterrupt:	#Ctrl + c
	GPIO.cleanup()
