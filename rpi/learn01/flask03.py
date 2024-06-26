from flask import Flask
import RPi.GPIO as GPIO
import time

app = Flask(__name__)
red_led = 21  # 빨간 LED의 제어핀

# GPIO 초기화
GPIO.setmode(GPIO.BCM)
GPIO.setup(red_led, GPIO.OUT)

# 기본 라우트
@app.route("/")
def hello():
    return """<h1>/led/on, /led/off로 접속하면 led를 on, off하는 웹페이지</h1>
              <p><a href='/led/on'>LED 켜기</a></p>
              <p><a href='/led/off'>LED 끄기</a></p>"""

# LED 제어 라우트
@app.route("/led/<action>")
def led_control(action):
    if action == "on":
        GPIO.output(red_led, GPIO.LOW)  # LED를 켬
        return "<h1>LED를 켭니다</h1>"
    elif action == "off":
        GPIO.output(red_led, GPIO.HIGH)  # LED를 끔
        return "<h1>LED를 끕니다</h1>"
    else:
        return "<h1>올바르지 않은 요청입니다</h1>", 404

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=10800, debug=True)
