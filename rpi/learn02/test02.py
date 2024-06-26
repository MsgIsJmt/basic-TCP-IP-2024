import RPi.GPIO as GPIO
import time

# GPIO 핀 번호 설정
segment_pins = [5, 6, 12, 16, 20, 13, 21]
button_pin = 4  # 예시로 버튼 핀 번호를 설정합니다

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
    GPIO.setup(button_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)  # 버튼 핀을 입력으로 설정 및 풀업 저항 사용

def display_number(number):
    pattern = segment_patterns[number]
    for pin, state in zip(segment_pins, pattern):
        GPIO.output(pin, state)  # 주어진 숫자에 해당하는 패턴을 각 핀에 출력

def main():
    current_number = 0  # 현재 표시할 숫자 초기화
    try:
        setup()  # GPIO 설정 초기화
        while True:
            button_state = GPIO.input(button_pin)  # 버튼 상태 읽기
            if button_state == GPIO.LOW:  # 버튼이 눌렸는지 확인
                current_number += 1  # 숫자 증가
                if current_number > 9:
                    current_number = 0  # 숫자가 9를 넘어가면 0으로 되돌리기
                display_number(current_number)  # 새로운 숫자 표시
                time.sleep(0.2)  # 디바운싱을 위해 짧은 시간 동안 대기
    except KeyboardInterrupt:
        print("\n사용자에 의해 프로그램이 종료되었습니다.")
    finally:
        GPIO.cleanup()  # 종료 전 GPIO 설정 초기화

if __name__ == '__main__':
    main()
