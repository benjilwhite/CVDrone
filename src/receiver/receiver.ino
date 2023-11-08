/*
 * Simple program that reads in an SBUS packet from an RC receiver
 * and controls the camera via PWM pan/tilt commands and I2C zoom/focus
 * commands.
 */
#include "sbus.h"
#include "Focuser.h"
#include <ESP32Servo.h>

/* Servo control objects */
ESP32PWM pwm;
Servo pan;
Servo tilt;

/* Arduino digital PWM pins */
int panPin = 3;
int tiltPin = 2;

/* Pulse width limits */
int minUs = 1000;
int maxUs = 2000;

/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial2, 0, 1, true);
bfs::SbusData data;

/* For zoom/focus control */
Focuser* focuser;

// Default channel values
int16_t prevChannels[] = {1500, 1500, 1500, 1500};
int16_t mappedChannels[4];

void setup() {
    /* Serial to display data */
    Serial.begin(115200);
    while (!Serial) {}

    /* Begin the SBUS communication */
    sbus_rx.Begin();

    // Begin camera lens I2C communication
    focuser = new Focuser();

    // Setup pan/tilt PWM communication
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    pan.setPeriodHertz(50);      // Standard 50hz RC PWM
	tilt.setPeriodHertz(50);
    pan.attach(panPin, minUs, maxUs);
	tilt.attach(tiltPin, minUs, maxUs);
}

void loop () {
    if (sbus_rx.Read()) {
        /* Grab the received data */
        data = sbus_rx.data();

        for(int8_t i = 0; i < 4; i++) {
            // If any of the inputs have changed by a threshold
            if(abs(data.ch[i] - prevChannels[i]) > 5) {
                // Remap SBUS channel values to nice, even numbers
                for(int8_t j = 0; j < 4; j++) {
                    mappedChannels[j] = map(data.ch[j], 174, 1811, 1000, 2000);
                }
                
                // Update pan
                if(mappedChannels[0] != prevChannels[0]) {
                    pan.writeMicroseconds(mappedChannels[0]);
                    prevChannels[0] = mappedChannels[0];
                }
                // Update tilt
                if(mappedChannels[1] != prevChannels[1]) {
                    // Limit tilt range so nothing breaks.
                    if(mappedChannels[1] < 1400) mappedChannels[1] = 1400;
                    if(mappedChannels[1] > 1600) mappedChannels[1] = 1600;
                    tilt.writeMicroseconds(mappedChannels[1]);
                    prevChannels[1] = mappedChannels[1];
                }
                // Update zoom
                if(mappedChannels[2] != prevChannels[2]) {
                    focuser->set(focuser->ZOOM, (mappedChannels[2] - 1000) * 20, true);
                    prevChannels[2] = mappedChannels[2];
                }
                // Update focus
                if(mappedChannels[3] != prevChannels[3]) {
                    focuser->set(focuser->FOCUS, (mappedChannels[3] - 1000) * 20, true);
                    prevChannels[3] = mappedChannels[3];
                }
                break;
            }
        }    
    }
}