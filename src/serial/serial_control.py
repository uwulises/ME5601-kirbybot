from asyncore import read
import serial
from serial import Serial
import time


class SerialControl:

    def __init__(self, port="/dev/ttyUSB0"):
        self.port = port
        self.serial = None

    def open_serial(self):
        try:
            self.serial = Serial(self.port, 9600, timeout=1, write_timeout=0.2)
            print("The port is available")
            serial_port = "Open"
            time.sleep(2)
        except serial.serialutil.SerialException:
            print("The port is at use")
            self.serial.close()
            self.serial.open()

    def close_serial(self):
        time.sleep(0.2)
        self.serial.close()

    def forward(self):
        self.serial.write('FW\n'.encode())

    def backward(self):
        self.serial.write('BW\n'.encode())

    def turn_right(self):
        self.serial.write('TR\n'.encode())

    def turn_left(self):
        self.serial.write('TL\n'.encode())

    def rise_brush(self):
        self.serial.write('RB\n'.encode())

    def place_brush(self):
        self.serial.write('PB\n'.encode())

    def run_brush(self):
        self.serial.write('run_b\n'.encode())

    def stop_brush(self):
        self.serial.write('stop_b\n'.encode())

    def call_ultrasonic_1(self):
        self.serial.write('HC1\n'.encode())
        self.read_from_arduino()
    
    def call_ultrasonic_2(self):
        self.serial.write('HC2\n'.encode())
        self.read_from_arduino()

    def read_from_arduino(self):
        msg = self.serial.readline().decode("utf-8").strip('\n').strip('\r')
        #print("MESSAGE: '{}'".format(msg))
        return msg
