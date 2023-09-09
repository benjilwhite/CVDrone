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
from Focuser import Focuser
    
def main():

    # establish a serial connection with the arduino
    # run the command "ls /dev/tty*" on the raspberry pi to see what port it is connected to
    ser = serial.Serial('/dev/ttyUSB0', baudrate = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS, timeout=1)

    # i2c_bus = 1
    focuser = Focuser(1)    
    
    while True:

        # Wait for the next input
        while ser.in_waiting == 0 and focuser.isBusy:
            time.sleep(1)

        line = ser.readline().decode('ascii').rstrip()

        # TODO: figure out these mappings and how they will be encoded/decoded.
        newZoomValue  = 0 # line[0:10]
        newFocusValue = 0 # line[10:20]
        newIRCut      = 0 # line[20:]

        focuser.set(Focuser.OPT_ZOOM, newZoomValue)
        focuser.set(Focuser.OPT_FOCUS, newFocusValue)
        focuser.set(Focuser.OPT_IRCUT, newIRCut & 1)

        ser.reset_input_buffer() # clear out any inputs that have been received since the last update
    

if __name__ == "__main__":
   main()
