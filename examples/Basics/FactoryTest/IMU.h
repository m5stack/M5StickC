


#define SH200I_ADDRESS 0x6C //7bit i2c address 
#define SH200I_WHOAMI 0x30
#define SH200I_ACC_CONFIG 0x0E
#define SH200I_GYRO_CONFIG 0x0F
#define SH200I_GYRO_DLPF 0x11
#define SH200I_FIFO_CONFIG 0x12
#define SH200I_ACC_RANGE 0x16
#define SH200I_GYRO_RANGE 0x2B
#define SH200I_OUTPUT_ACC_X 0x00
#define SH200I_OUTPUT_GYRO_X 0x06
#define SH200I_OUTPUT_TEMP 0x0C
#define SH200I_REG_SET1 0xBA
#define SH200I_REG_SET2 0xCA   //ADC reset
#define SH200I_ADC_RESET  0xC2   //drive reset
#define SH200I_SOFT_RESET 0x7F
#define SH200I_RESET 0x75



extern void I2C_Read_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *read_Buffer);
extern void I2C_Write_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *write_Buffer);
extern void sh200i_ADCReset(void);
extern void sh200i_Reset(void);
extern void sh200i_init(void);

