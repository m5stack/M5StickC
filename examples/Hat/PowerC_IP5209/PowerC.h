#include "Arduino.h"


class POWERC
{
    public:
        void Init();
        float ReadVoltage();
        float ReadCurrent();
    private:
        void Write1Byte(uint8_t address,uint8_t Register_address,uint8_t data);
        void Write2Byte(uint8_t address,uint8_t Register_address,uint16_t data);
        void WriteBytes(uint8_t address,uint8_t Register_address,uint8_t * data, size_t size);
        uint8_t ReadBytes(uint8_t address, uint8_t subAddress, uint8_t count,uint8_t * dest);
};