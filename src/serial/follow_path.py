from serial_control import SerialControl
import time



robot = SerialControl(port="/dev/cu.usbserial-14140")
robot.open_serial()

def call_mpu():
    IMU_msg = robot.read_imu()



