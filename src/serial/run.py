from serial_control import SerialControl
import time

robot_serial = SerialControl(port="/dev/cu.usbserial-14140")
robot_serial.open_serial()

while True:
    robot_serial.forward()
    time.sleep(0.01)
