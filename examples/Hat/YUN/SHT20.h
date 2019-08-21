#ifndef SHT20_h
#define SHT20_h

#include <Arduino.h>
#include <Wire.h>

#define SHT20_ADDR                      0x40

#define TRIGGER_TEMP_MEASURE_HOLD       0xE3
#define TRIGGER_HUMD_MEASURE_HOLD       0xE5
#define TRIGGER_TEMP_MEASURE_NOHOLD     0xF3
#define TRIGGER_HUMD_MEASURE_NOHOLD     0xF5
#define WRITE_USER_REG                  0xE6
#define READ_USER_REG                   0xE7
#define SOFT_RESET                      0xFE
#define REG_RESOLUTION_MASK             0x81
#define REG_RESOLUTION_RH12_TEMP14      0x00
#define REG_RESOLUTION_RH8_TEMP12       0x01
#define REG_RESOLUTION_RH10_TEMP13      0x80
#define REG_RESOLUTION_RH11_TEMP11      0x81
#define REG_END_OF_BATTERY              0x40
#define REG_HEATER_ENABLED              0x04
#define REG_DISABLE_OTP_RELOAD          0x02
#define MAX_WAIT                        100
#define DELAY_INTERVAL                  10
#define SHIFTED_DIVISOR                 0x988000

class SHT20  
{
public:
    void set_resolution(uint8_t resBits);
    float read_humidity(void);
    float read_temperature(void);
    void write_register(uint8_t val);
    uint8_t read_register(void);

private:
    bool check_crc(uint16_t data_in, uint8_t crc);
    bool I2C_read_u16(uint8_t reg, uint16_t *val);
};

#endif