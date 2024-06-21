import RPi.GPIO as GPIO
import time

def measure_distance():
    GPIO.output(trigPin, True)  # 10us 동안 high 레벨로 trigger 출력하여 초음파 발생 준비
    time.sleep(0.00001)
    GPIO.output(trigPin, False)

    while GPIO.input(echoPin) == 0:  # echo가 없으면
        pulse_start = time.time()  # 펄스가 시작된 시간 기록
    while GPIO.input(echoPin) == 1:  # echo가 있으면
        pulse_end = time.time()  # 펄스가 끝난 시간 기록

    pulse_duration = pulse_end - pulse_start  # 펄스의 지속 시간 계산
    distance = pulse_duration * 17150  # 거리 계산 (소리의 속도는 343m/s 이므로 * 34300 / 2)
    distance = round(distance, 2)  # 거리를 소수점 둘째 자리까지 반올림

    return distance  # 거리 반환

def play_continuous_tone(distance):
    if distance < 10:
        frequency = 2000
    elif distance < 20:
        frequency = 1500
    elif distance < 30:
        frequency = 1000
    elif distance < 40:
        frequency = 750
    elif distance < 50:
        frequency = 500
    elif distance < 60:
        frequency = 250
    else:
        frequency = 0

    if frequency > 0:
        p = GPIO.PWM(buzzerPin, frequency)
        p.start(50)  # 50%의 듀티 사이클로 PWM 시작
        time.sleep(0.1)  # 소리를 재생할 시간 설정
        p.stop()  # PWM 중지
    else:
        time.sleep(0.1)  # 거리가 60cm 이상일 때 소리를 발생시키지 않고 대기

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
        distance = measure_distance()
        print("Distance: %.2f cm" % distance)

        play_continuous_tone(distance)

        time.sleep(0.1)  # 거리를 연속적으로 측정하고 소리를 재생하기 위해 짧은 시간 동안 대기

except KeyboardInterrupt:
    GPIO.cleanup()
