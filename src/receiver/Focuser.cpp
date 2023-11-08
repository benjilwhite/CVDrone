#include "Arduino.h"
#include "Focuser.h"
#include <I2C_16Bit.h>

Focuser::Focuser() {
    I2C_16Bit_begin();
}

uint16_t Focuser::read(uint8_t chip_addr, uint8_t reg_addr) {
    return I2C_16Bit_readFromModule(chip_addr, reg_addr);
}

void Focuser::write(uint8_t chip_addr, uint8_t reg_addr, uint16_t value) {
    I2C_16Bit_writeToModule(chip_addr, reg_addr, value);
}

bool Focuser::isBusy(void) {
    return this->read(CHIP_I2C_ADDR, BUSY_REG_ADDR) != 0;
}

// Wait a certain amount of time for the focuser to be free
void Focuser::waitingForFree(void) {
    uint16_t count = 0;
    while(this->isBusy() && count < 500) {
        count++;
        delay(10);
    }
}

void Focuser::reset(ControlOption info, bool waitForCompletion) {
    this->waitingForFree();
    if(info.RESET_ADDR == -1) return;
    this->write(CHIP_I2C_ADDR, info.RESET_ADDR, 0x0);
    if (waitForCompletion) {
        this->waitingForFree();
    }
}

uint16_t Focuser::get(ControlOption info) {
    this->waitingForFree();
    return read(CHIP_I2C_ADDR, info.REG_ADDR);
}

void Focuser::set(ControlOption info, uint16_t value, bool waitForCompletion) {
    this->waitingForFree();
    if(value > info.MAX_VALUE) {
        value = info.MAX_VALUE;
    }
    if(value < info.MIN_VALUE) {
        value = info.MIN_VALUE;
    }
    this->write(CHIP_I2C_ADDR, info.REG_ADDR, value);
    if (waitForCompletion) {
        this->waitingForFree();
    }
}
