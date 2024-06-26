import RPi.GPIO as GPIO
import time

# GPIO 핀 번호 설정
segment_pins = [5, 6, 12, 16, 20, 13, 21]

# 각 숫자에 해당하는 7세그먼트 패턴 (0 ~ 9)
segment_patterns = [
    [0, 0, 0, 0, 0, 0, 1],  # 0
    [1, 0, 0, 1, 1, 1, 1],  # 1
    [0, 0, 1, 0, 0, 1, 0],  # 2
    [0, 0, 0, 0, 1, 1, 0],  # 3
    [1, 0, 0, 1, 1, 0, 0],  # 4
    [0, 1, 0, 0, 1, 0, 0],  # 5
    [0, 1, 0, 0, 0, 0, 0],  # 6
    [0, 0, 0, 1, 1, 1, 1],  # 7
    [0, 0, 0, 0, 0, 0, 0],  # 8
    [0, 0, 0, 0, 1, 0, 0]   # 9
]

def setup():
    GPIO.setmode(GPIO.BCM)  # GPIO 핀 번호를 BCM 방식으로 설정
    for pin in segment_pins:
        GPIO.setup(pin, GPIO.OUT)  # 모든 세그먼트 핀을 출력으로 설정

def display_number(number):
    pattern = segment_patterns[number]
    for pin, state in zip(segment_pins, pattern):
        GPIO.output(pin, state)  # 주어진 숫자에 해당하는 패턴을 각 핀에 출력

def main():
    try:
        setup()  # GPIO 설정 초기화
        while True:
            for number in range(10):
                display_number(number)  # 0부터 9까지 숫자를 7세그먼트에 표시
                time.sleep(1)  # 각 숫자를 1초 동안 표시
    except KeyboardInterrupt:
        print("\n사용자에 의해 프로그램이 종료되었습니다.")
    finally:
        GPIO.cleanup()  # 종료 전 GPIO 설정 초기화

if __name__ == '__main__':
    main()
