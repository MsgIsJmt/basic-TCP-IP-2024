# 동일한 폴더 위치에 templates 폴더를 만들고 거기에 html파일을 저장한다.

from flask import Flask, request, render_template
import RPi.GPIO as GPIO

app = Flask(__name__)

# GPIO 핀 21번에 연결된 LED 객체 초기화
red_led = 21

GPIO.setmode(GPIO.BCM)
GPIO.setup(red_led, GPIO.OUT)

# 홈 페이지를 렌더링하는 루트
@app.route('/')
def home():
    return render_template("index.html")

# index.html에서 폼 제출을 처리하는 루트
@app.route('/data', methods=['POST'])
def data():
    # 폼에서 'led'의 값을 가져옴
    data = request.form['led']
    
    # 'led' 값에 따라 해당 작업 수행
    if data == 'on':
    	GPIO.output(red_led, False)  # LED 켜기
    	return home()
        
    elif data == 'off':
    	GPIO.output(red_led, True)  # LED 끄기
    	return home()

if __name__ == '__main__':
    # Flask 앱을 IP '0.0.0.0' (모든 네트워크 인터페이스)와 포트 '80' (표준 HTTP 포트)에서 실행
    app.run(host='0.0.0.0', port=80)

