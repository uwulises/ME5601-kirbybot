from serial_control import SerialControl
import time
from pynput.keyboard import Key, Listener, KeyCode

robot = SerialControl(port="/dev/cu.usbserial-14140")
robot.open_serial()


def on_press(key):
    if key == Key.up:
        robot.forward()
    if key == Key.down:
        robot.backward()
    if key == Key.left:
        robot.turn_left()
    if key == Key.right:
        robot.turn_right()
    if key == KeyCode.from_char('q'):
        #turn on the brush
        robot.run_brush()
    if key == KeyCode.from_char('e'):
        #turn on the brushe
        robot.stop_brush()
    if key == KeyCode.from_char('r'):
        #down the brush
        robot.place_brush()
    if key == KeyCode.from_char('f'):
        #rise the brush
        robot.rise_brush()

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
