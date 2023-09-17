'''
    Simple funcion to read in serial inputs from and arduino, and send the
    focus/zoom values to the Arducam PTZ camera.

    More info at https://github.com/ArduCAM/PTZ-Camera-Controller
'''

import serial 
import os
import sys
import time
import argparse
import pigpio
from Focuser import Focuser

def main():

    # establish a serial connection with the arduino
    # run the command "ls /dev/tty*" on the raspberry pi to see what port it is connected to
    ser = serial.Serial('/dev/ttyUSB0', baudrate = 115200, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS, timeout=1)

    # i2c_bus = 1
    focuser = Focuser(1)    
    prevZoomValue = 0
    prevFocusValue = 0
    
    # Setup the gimbal controls
    pan_pin = 14
    tilt_pin = 15
    pi = pigpio.pi()

    while True:

        # Wait for the next input
        while ser.in_waiting == 0 and focuser.isBusy:
            time.sleep(1)

        # Split the inputs into a list
        line = ser.readline().decode('ascii').rstrip()
        split_values = line.split(":")
        values = [int(value) for value in split_values]

        # update the gimbal position
        # TODO: map the values to the correct servo values.
        pi.set_servo_pulsewidth(pan_pin, values[0])
        pi.set_servo_pulsewidth(tilt_pin, values[1])

        newZoomValue  = values[2] # line[0:10]
        newFocusValue = values[3] # line[10:20]

        focuser.set(Focuser.OPT_ZOOM, newZoomValue)
        focuser.set(Focuser.OPT_FOCUS, newFocusValue)

        ser.reset_input_buffer() # clear out any inputs that have been received since the last update
    

if __name__ == "__main__":
   main()
