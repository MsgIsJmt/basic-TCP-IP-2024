import RPi.GPIO as GPIO
import time

# GPIO 핀 번호 설정
switch = 6  # 스위치는 GPIO 핀 6에 연결
red_led = 21  # 빨강 LED는 GPIO 핀 21번에 연결
blue_led = 20  # 파랑 LED는 GPIO 핀 20번에 연결
green_led = 26  # 초록 LED는 GPIO 핀 26번에 연결

# GPIO 설정
GPIO.setmode(GPIO.BCM)
GPIO.setup(switch, GPIO.IN)
GPIO.setup(red_led, GPIO.OUT)
GPIO.setup(blue_led, GPIO.OUT)
GPIO.setup(green_led, GPIO.OUT)

led_state = 0  # LED 상태를 나타내는 변수 (0: 꺼짐, 1: 빨강, 2: 파랑, 3: 초록)
led_count = 0  # 스위치 누름 횟수를 세는 변수

try:
    while True:
        if GPIO.input(switch) == GPIO.HIGH:  # 스위치가 눌렸을 때 (HIGH 상태)
            led_count += 1
            print(f"스위치를 {led_count}번 눌렀습니다.")

            if led_state == 0:
                GPIO.output(red_led, GPIO.LOW)
                GPIO.output(blue_led, GPIO.LOW)
                GPIO.output(green_led, GPIO.HIGH)
                led_state = 1

            elif led_state == 1:
                 GPIO.output(red_led, GPIO.HIGH)
                 GPIO.output(blue_led, GPIO.LOW)
                 GPIO.output(green_led, GPIO.LOW)
                 led_state = 2

            elif led_state == 2:
                 GPIO.output(red_led, GPIO.LOW)
                 GPIO.output(blue_led, GPIO.HIGH)
                 GPIO.output(green_led, GPIO.LOW)
                 led_state = 3


            elif led_state == 3:
                GPIO.output(red_led, GPIO.HIGH)
                GPIO.output(blue_led, GPIO.HIGH)
                GPIO.output(green_led, GPIO.HIGH)
                led_state = 0

            time.sleep(0.5)  # 딜레이를 줘서 여러 번 눌러도 빠르게 반응하게 함

except KeyboardInterrupt:
    pass

finally:
    GPIO.cleanup()
