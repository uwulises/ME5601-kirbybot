from serial_control import SerialControl
import time
from pynput.keyboard import Key, Listener

robot_serial = SerialControl(port="/dev/cu.usbserial-14140")
robot_serial.open_serial()


def on_press(key):
    if key == Key.up:
        robot_serial.forward()
    if key == Key.down:
        robot_serial.backward()
    if key == Key.left:
        robot_serial.turn_left()
    if key == Key.right:
        robot_serial.turn_right()


def on_release(key):
    print('{0} release'.format(
        key))
    if key == Key.esc:
        # Stop listener
        return False


# Collect events until released
with Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()
