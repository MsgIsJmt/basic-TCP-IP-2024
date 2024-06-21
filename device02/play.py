import RPi.GPIO as GPIO
import time
from pygame import mixer

# 소리 센서 핀 설정
soundSensorPin = 13

song_path = "/home/pi/Music/my_song.mp3" 

# GPIO 설정
GPIO.setmode(GPIO.BCM)
GPIO.setup(soundSensorPin, GPIO.IN)

# Pygame 초기화 및 mixer 설정
mixer.init()
mixer.music.load(song_path)

try:
    while True:
        if GPIO.input(soundSensorPin) == GPIO.HIGH:
            print("Sound detected! Playing song...")
            mixer.music.play()
            while mixer.music.get_busy():
                time.sleep(1)
        else:
            time.sleep(0.1)

except KeyboardInterrupt:
    print("\nExiting program")
    mixer.music.stop()
    GPIO.cleanup()
