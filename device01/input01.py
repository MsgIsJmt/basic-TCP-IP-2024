import RPi.GPIO as GPIO
import time

switch = 6  # 스위치는 GPIO 핀 6에 연결

GPIO.setmode(GPIO.BCM)
GPIO.setup(switch, GPIO.IN)

try:
    while True:
        if GPIO.input(switch) == True:  # 스위치가 눌렸을 때 (HIGH 상태)
            print("Pushed")
            time.sleep(0.1)  # 주어진  시간 동안 대기
except KeyboardInterrupt:

    GPIO.cleanup()


