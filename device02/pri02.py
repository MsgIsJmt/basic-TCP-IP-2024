import RPi.GPIO as GPIO
import time

priPin = 24  # PIR 센서의 출력 핀
red_led = 21  # 빨간 LED의 제어 핀

GPIO.setmode(GPIO.BCM)
GPIO.setup(priPin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)  # PIR 센서 입력 설정, pull-down 사용
GPIO.setup(red_led, GPIO.OUT)  # 빨간 LED 출력 설정

try:
    while True:
        if GPIO.input(priPin):  # 만약 PIR 센서에서 신호가 감지되면 (True)
            GPIO.output(red_led, False)  # 빨간 LED를 끔 (High에서 Low로)
        else:  # PIR 센서에서 신호가 감지되지 않으면 (False)
            GPIO.output(red_led, True)  # 빨간 LED를 켬 (Low에서 High로)
            time.sleep(2)  # 2초 동안 대기

except KeyboardInterrupt:
    GPIO.cleanup()  # 프로그램 종료 시 GPIO 리소스 정리
