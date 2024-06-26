from gpiozero import Button  # gpiozero에서 Button 클래스를 임포트
import time  # time 모듈 임포트

# GPIO 핀 22번에 연결된 버튼 객체 생성
swPin = Button(22)

# 이전 스위치 상태(oldSw)와 현재 스위치 상태(newSw) 초기화
oldSw = 0
newSw = 0

try:
    while True:  # 무한 반복
        newSw = swPin.is_pressed  # 현재 버튼 상태를 가져옴

        if newSw != oldSw:  # 이전 상태와 현재 상태가 다른 경우에만 실행
            oldSw = newSw  # 이전 상태를 현재 상태로 업데이트

            if newSw == 1:  # 버튼이 눌렸을 때
                print("click")  # "click" 메시지 출력

            time.sleep(0.2)  # 0.2초 대기 후 다시 반복

except KeyboardInterrupt:
    pass  # 사용자가 Ctrl+C를 눌러 프로그램을 중지할 때 예외 처리
