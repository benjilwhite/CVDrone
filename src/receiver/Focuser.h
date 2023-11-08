#ifndef FOCUSER_H
#define FOCUSER_H

#include "Arduino.h"

class Focuser {
    public:   

        // Stores information about a potential move
        struct ControlOption {
            uint8_t REG_ADDR;
            int MIN_VALUE;
            int MAX_VALUE;
            int DEF_VALUE;
            uint8_t RESET_ADDR;
        };

        // Wait a certain amount of time for the focuser to be free
        Focuser(void);
        void waitingForFree(void);
        void reset(ControlOption info, bool waitForCompletion);
        void set(ControlOption info, uint16_t value, bool waitForCompletion);
        uint16_t get(ControlOption info);
        bool isBusy(void);

        ControlOption ZOOM = { 0x01,
                               0,
                               20000,
                               0,
                               0x01 + 0x0A };
        ControlOption FOCUS = { 0x0,
                                0,
                                20000,
                                0,
                                0x0A };
        
    private:

        // The user shouldn't be able to call these functions without proper checks
        uint16_t read(uint8_t chip_addr, uint8_t reg_addr);
        void write(uint8_t chip_addr, uint8_t reg_addr, uint16_t value);

        // I2C Addresses
        uint8_t CHIP_I2C_ADDR = 0x0C;
        uint8_t BUSY_REG_ADDR = 0x04;        
};
#endif