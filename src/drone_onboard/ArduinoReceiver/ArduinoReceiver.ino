//
// Simple program that reads in an SBUS packet and outputs it via serial
// whenever something changes
//
#include "sbus.h"

/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial2, 0, 1, true);

/* SBUS data */
bfs::SbusData data;

// Default channel values
int16_t prevChannels[] = {1500, 1500, 1500, 1500};
int16_t mappedChannels[4];

void setup() {
  /* Serial to display data */
  Serial.begin(115200);
  while (!Serial) {}

  /* Begin the SBUS communication */
  sbus_rx.Begin();
}

void loop () {
  if (sbus_rx.Read()) {
    /* Grab the received data */
    data = sbus_rx.data();

    for(int8_t i = 0; i < 4; i++) {
      // Only send serial data if the received data has changed
      if(abs(data.ch[i] - prevChannels[i]) > 5) {
        for(int8_t j = 0; j < 4; j++) {
          prevChannels[j] = data.ch[j]; //update the previous data
          mappedChannels[j] = map(data.ch[j], 174, 1811, 1000, 2000);
        }
        String output = String(mappedChannels[0]) + ":" + String(mappedChannels[1]) + ":" + String(mappedChannels[2]) + ":" + String(mappedChannels[3]);
        Serial.println(output);//174-1811
        break;
      }
    }    
  }
}