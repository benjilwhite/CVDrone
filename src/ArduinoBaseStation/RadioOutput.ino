#include <HardwareSerial.h>
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
    Serial.begin(9600);

    sbusSerial.begin(100000, SERIAL_8E2, SERIAL1_RX, SERIAL1_TX, false, 100UL);

    xTaskCreatePinnedToCore(
        ioTaskHandler, /* Function to implement the task */
        "outputTask",  /* Name of the task */
        10000,         /* Stack size in words */
        NULL,          /* Task input parameter */
        0,             /* Priority of the task */
        &ioTask,       /* Task handle. */
        0);

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

//Configure the program outputs
void outputSubtask()
{
    for(uint8_t i = 0; i < SBUS_CHANNEL_COUNT; i++) {
      output.channels[i] += 1;
      if(output.channels[i] >= 2000) {
        output.channels[i] = 1000;
      }
    }
}

void ioTaskHandler(void *pvParameters)
{
    portTickType xLastWakeTime;
    const portTickType xPeriod = OUTPUT_UPDATE_TASK_MS / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();

    while (true)
    {
        outputSubtask();

        // Put task to sleep
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
    vTaskDelete(NULL);
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
}