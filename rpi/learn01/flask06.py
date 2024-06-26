from picamera2 import Picamera2  # picamera2 모듈에서 Picamera2 클래스를 임포트
import time  # time 모듈 임포트

# Picamera2 객체 생성
picam2 = Picamera2()

# 미리보기 설정(configuration) 생성
camera_config = picam2.create_preview_configuration()

# 카메라에 설정(configuration) 적용
picam2.configure(camera_config)

# 카메라 시작
picam2.start()

# 2초 대기
time.sleep(2)

# 이미지 캡처하여 파일로 저장
picam2.capture_file("test1.jpg")
