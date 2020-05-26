from picamera import PiCamera
from time import sleep
camera=PiCamera()
camera.start_preview()
sleep(2)
camera.capture('/home/pi/Desktop/test.jpg')
camera.stop_preview()