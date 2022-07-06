from serial_control import SerialControl
import time


robot = SerialControl(port="/dev/tty.usbmodem142401")
#robot = SerialControl(port="/dev/tty.HC06")
robot.open_serial()




while True:
    robot.forward()
    time.sleep(0.1)

