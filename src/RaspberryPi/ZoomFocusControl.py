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
    prevPanValue = 0
    prevTiltValue = 0
    
    # Setup the gimbal controls
    pan_pin = 18
    tilt_pin = 17
    pi = pigpio.pi()

    while True:

        # Read until we get to the latest, most recent line
        line = 'empty'
        while ser.in_waiting > 0:
            line = ser.readline().decode('ascii').rstrip()
    
        # Only proceed if data was read
        if line == 'empty':
            continue

        # Split the inputs into a list
        split_values = line.split(":")
        values = [int(value) for value in split_values]

        if values[0] != prevPanValue or values[1] != prevTiltValue:
            # Update the gimbal position
            if values[1] < 1400:
                values[1] = 1400
            elif values[1] > 1600:
                values[1] = 1600
        
            pi.set_servo_pulsewidth(pan_pin, values[0])
            pi.set_servo_pulsewidth(tilt_pin, values[1])
            prevPanValue = values[0]
            prevTiltValue = values[1]

        if not focuser.isBusy and (values[2] != prevZoomValue or values[3] != prevFocusValue):
            # Update zoom/focus
            prevZoomValue = values[2]
            prevFocusValue = values[3]
            focuser.set(Focuser.OPT_ZOOM, (newZoomValue - 1000) * 25)
            focuser.set(Focuser.OPT_FOCUS, (newFocusValue - 1000) * 25)
        

if __name__ == "__main__":
   main()
