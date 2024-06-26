from gpiozero import Button
import time

swPin = Button(14)

oldSw = 0
newSw = 0

try:
    while True:
        newSw = swPin.is_pressed
        if newSw != oldSw:
            oldSw = newSw
            
            if newSw == 1:
                print("click")
                
            time.sleep(0.2)

except KeyboardInterrupt:
    pass
