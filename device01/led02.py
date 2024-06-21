import RPi.GPIO as GPIO
import time

# GPIO 핀 번호 설정
red_led = 21  # 빨강 LED는 GPIO 핀 21번에 연결
blue_led = 26  # 파랑 LED는 GPIO 핀 26번에 연결
green_led = 4  # 녹색 LED는 GPIO 핀 4번에 연결

# GPIO 설정
GPIO.setmode(GPIO.BCM)
GPIO.setup(red_led, GPIO.OUT)
GPIO.setup(blue_led, GPIO.OUT)
GPIO.setup(green_led, GPIO.OUT)

try:
    while True:
        # 빨강 LED 켜기
        GPIO.output(red_led, GPIO.HIGH)
        time.sleep(1)  # 1초 대기
        GPIO.output(red_led, GPIO.LOW)  # 빨강 LED 끄기

        # 파랑 LED 켜기
        GPIO.output(blue_led, GPIO.HIGH)
        time.sleep(1)
        GPIO.output(blue_led, GPIO.LOW)

        # 녹색 LED 켜기
        GPIO.output(green_led, GPIO.HIGH)
        time.sleep(1)
        GPIO.output(green_led, GPIO.LOW)

except KeyboardInterrupt:
    pass

finally:
    GPIO.cleanup()  # GPIO 정리

