#include <HardwareSerial.h>
#include "StringSplitter.h"
#include "sbus.h"

#define OUTPUT_UPDATE_TASK_MS 20
#define SBUS_UPDATE_TASK_MS 15
#define SERIAL_TASK_MS 50
#define SERIAL1_RX 2
#define SERIAL1_TX 1
#define SBUS_CHANNEL_COUNT 16
#define DEFAULT_CHANNEL_VALUE 1500

struct dataOutput_t {
    uint16_t channels[SBUS_CHANNEL_COUNT];
};
uint8_t sbusPacket[SBUS_PACKET_LENGTH] = {0};
HardwareSerial sbusSerial(1);
dataOutput_t output;
uint32_t nextSbusTaskMs = 0;
TaskHandle_t ioTask;

uint32_t channelVal = 0;  //temp testing value

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(5); //serial should be fast enough to transmit everything in this time. if not, increase this value.

    sbusSerial.begin(100000, SERIAL_8E2, SERIAL1_RX, SERIAL1_TX, false, 100UL);

    // Set channels to default values
    for (uint8_t i = 0; i < SBUS_CHANNEL_COUNT; i++)
    {
        output.channels[i] = DEFAULT_CHANNEL_VALUE;
    }
}

//I do not get function pointers to object methods, no way...
int getRcChannel_wrapper(uint8_t channel)
{
    if (channel >= 0 && channel < SBUS_CHANNEL_COUNT)
    {
        return output.channels[channel];
    }
    else
    {
        return DEFAULT_CHANNEL_VALUE;
    }
}

void loop()
{
    /* 
     * Send Trainer data in SBUS stream
     */
    if (millis() > nextSbusTaskMs)
    {
        sbusPreparePacket(sbusPacket, false, false, getRcChannel_wrapper);
        sbusSerial.write(sbusPacket, SBUS_PACKET_LENGTH);

        nextSbusTaskMs = millis() + SBUS_UPDATE_TASK_MS;
    }

    /*
     * Get new data via serial
     */
    if(Serial.available()) {
        String inputs = Serial.readString();  //read until timeout
        inputs.trim();                        // remove any \r \n whitespace at the end of the String

        StringSplitter *splitter = new StringSplitter(inputs, ':', 4);  // Get the four input values

        output.channels[0] = (uint16_t)splitter->getItemAtIndex(0).toInt();
        output.channels[1] = (uint16_t)splitter->getItemAtIndex(1).toInt();
        output.channels[2] = (uint16_t)splitter->getItemAtIndex(2).toInt();
        output.channels[3] = (uint16_t)splitter->getItemAtIndex(3).toInt();
    }
}