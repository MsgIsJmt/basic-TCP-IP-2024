import RPi.GPIO as GPIO
import time

steps = [21, 22, 23, 24]
GPIO.setmode(GPIO.BCM)

# 각 스텝 핀을 출력으로 설정하고 초기 LOW로 설정
for stepPin in steps:
    GPIO.setup(stepPin, GPIO.OUT)
    GPIO.output(stepPin, 0)

ranges = [[0, 0, 0, 1], [0, 0, 1, 0], [0, 1, 0, 0], [1, 0, 0, 0]]

try:
    while True:
        for stepPin in range(0, 4):
            for i in range(0, len(ranges[stepPin])):
                GPIO.output(steps[i], ranges[stepPin][i])
            time.sleep(0.01)

except KeyboardInterrupt:
    GPIO.cleanup()
