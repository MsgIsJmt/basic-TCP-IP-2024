import RPi.GPIO as GPIO
import time

def measure():
    GPIO.output(trigPin, True)  # 10us 동안 high 레벨로 trigger 출력하여 초음파 발생 준비
    time.sleep(0.00001)
    GPIO.output(trigPin, False)
    start = time.time()  # 현재 시간 저장

    while GPIO.input(echoPin) == 0:  # echo가 없으면
        start = time.time()  # 현재 시간을 start 변수에 저장
    while GPIO.input(echoPin) == 1:  # echo가 있으면
        stop = time.time()  # 현재 시간을 stop 변수에 저장

    elapsed = stop - start  # 걸린 시간을 구하고
    distance = (elapsed * 34300) / 2  # 초음파 속도를 이용해서 거리 계산 (소리의 속도는 34300 cm/s)

    return distance  # 거리 반환

def play_sound(distance):
    if distance < 5:  # 거리가 5cm 미만이면
        frequency = 2500  # 더 높은 주파수로 소리를 내도록 설정
    elif distance < 10:  # 거리가 10cm 미만이면
        frequency = 2000
    elif distance < 15:  # 거리가 15cm 미만이면
        frequency = 1750
    elif distance < 20:  # 거리가 20cm 미만이면
        frequency = 1500
    elif distance < 25:  # 거리가 25cm 미만이면
        frequency = 1250
    elif distance < 30:  # 거리가 30cm 미만이면
        frequency = 1000
    else:
        frequency = 500  # 그 외의 경우

    p = GPIO.PWM(buzzerPin, frequency)
    p.start(50)  # 50%의 듀티 사이클로 PWM 시작
    time.sleep(0.1)  # 소리를 재생할 시간 설정
    p.stop()  # PWM 중지

# 핀 설정
trigPin = 27
echoPin = 17
buzzerPin = 13

GPIO.setmode(GPIO.BCM)
GPIO.setup(trigPin, GPIO.OUT)
GPIO.setup(echoPin, GPIO.IN)
GPIO.setup(buzzerPin, GPIO.OUT)

try:
    while True:
        distance = measure()
        print("Distance: %.2f cm" % distance)
        play_sound(distance)  # 거리에 따라 소리를 재생
        time.sleep(1)

except KeyboardInterrupt:
    GPIO.cleanup()
