import RPi.GPIO as GPIO
import time

button_pin = 4  # 버튼 핀 번호 설정

def setup():
    GPIO.setmode(GPIO.BCM)  # GPIO 핀 번호를 BCM 방식으로 설정
    GPIO.setup(button_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)  # 버튼 핀을 입력으로 설정 및 풀업 저항 사용

def main():
    try:
        setup()  # GPIO 설정 초기화
        print("버튼 테스트를 시작합니다. 버튼을 눌러보세요...")
        while True:
            button_state = GPIO.input(button_pin)  # 버튼 상태 읽기
            if button_state == GPIO.LOW:  # 버튼이 눌린 경우
                print("버튼이 눌렸습니다.")
                while GPIO.input(button_pin) == GPIO.LOW:
                    time.sleep(0.1)  # 버튼이 떨어질 때까지 대기
                print("버튼이 떨어졌습니다.")
                time.sleep(0.2)  # 디바운싱을 위해 잠시 대기
    except KeyboardInterrupt:
        print("\n사용자에 의해 프로그램이 종료되었습니다.")
    finally:
        GPIO.cleanup()  # 종료 전 GPIO 설정 초기화

if __name__ == '__main__':
    main()
