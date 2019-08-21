#include "SHT20.h"

static bool I2C_readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest) {
    Wire.beginTransmission(address);   // Initialize the Tx buffer
    Wire.write(subAddress);            // Put slave register address in Tx buffer
    if(Wire.endTransmission() != 0) {
        return false;
    }

    uint8_t time_count = 0;
    while(Wire.requestFrom(address, (uint8_t)count) != count) {
        if(time_count++ > 10) {
            return false;
        }
        delay(10);
    }

    uint8_t i = 0;
    while (Wire.available()) {
        dest[i++] = Wire.read();// Put read results in the Rx buffer
    }
    return true;
}

bool SHT20::check_crc(uint16_t data_in, uint8_t crc) {
    uint32_t remainder = data_in << 8;
    uint32_t divsor = SHIFTED_DIVISOR;

    remainder |= crc;
    for(int i = 0 ; i < 16 ; i++){
        if(remainder & (uint32_t)1 << (23 - i)){
            remainder ^= divsor;
        }
        divsor >>= 1;
    }
    
    return remainder == 0;
}

bool SHT20::I2C_read_u16(uint8_t reg, uint16_t* val) {
    uint8_t buffer[3] = { 0x00 };

    if(I2C_readBytes(SHT20_ADDR, reg, 3, buffer) != true) {
        return false;
    }

    uint16_t rawValue = (buffer[0] << 8) |  buffer[1];
    if(check_crc(rawValue, buffer[2]) != true){
        return false;
    }
    *val = rawValue & 0xFFFC;
    return true;
}

float SHT20::read_humidity(void) {
    uint16_t rawHumidity = 0;

    if(I2C_read_u16(TRIGGER_HUMD_MEASURE_NOHOLD, &rawHumidity) != true) {
        return 0;
    }

    float hum = - 6.0 + rawHumidity * (125.0 / 65536.0);
    return hum;
}

float SHT20::read_temperature(void) {
    uint16_t rawTemperature = 0;
    
    if(I2C_read_u16(TRIGGER_TEMP_MEASURE_NOHOLD, &rawTemperature) != true) {
        return 0;
    }

    float tempTemperature = - 46.85 + (float)rawTemperature * (175.72 / 65536.0);
    return tempTemperature;
}

void SHT20::set_resolution(uint8_t resolution) {
    byte userRegister = read_register();
    userRegister &= B01111110;
    resolution &= B10000001;
    userRegister |= resolution;
    write_register(userRegister);
}

uint8_t SHT20::read_register(void) {
    uint8_t reg = 0x00;
    I2C_readBytes(SHT20_ADDR, READ_USER_REG, 1, &reg);
    return reg;
}

void SHT20::write_register(byte val) {
    Wire.beginTransmission(SHT20_ADDR);
    Wire.write(WRITE_USER_REG);
    Wire.write(val);
    Wire.endTransmission();
}
