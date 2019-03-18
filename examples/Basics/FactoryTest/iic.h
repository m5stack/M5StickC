#ifndef __IIC_H__
#define __IIC_H__
#include <driver/i2c.h>


#define DATA_LENGTH                        512              /*!<Data buffer length for test buffer*/
#define RW_TEST_LENGTH                     129              /*!<Data length for r/w test, any value from 0-DATA_LENGTH*/
#define DELAY_TIME_BETWEEN_ITEMS_MS        1234             /*!< delay time between different test items */

#define I2C_EXAMPLE_SLAVE_SCL_IO           GPIO_NUM_13               /*!<gpio number for i2c slave clock  */
#define I2C_EXAMPLE_SLAVE_SDA_IO           GPIO_NUM_4               /*!<gpio number for i2c slave data */
#define I2C_EXAMPLE_SLAVE_NUM              I2C_NUM_0        /*!<I2C port number for slave dev */
#define I2C_EXAMPLE_SLAVE_TX_BUF_LEN       (2*DATA_LENGTH)  /*!<I2C slave tx buffer size */
#define I2C_EXAMPLE_SLAVE_RX_BUF_LEN       (2*DATA_LENGTH)  /*!<I2C slave rx buffer size */

#define I2C_EXAMPLE_MASTER_SCL_IO          GPIO_NUM_33               /*!< gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO          GPIO_NUM_32               /*!< gpio number for I2C master data  */
#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_1        /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_FREQ_HZ         100000           /*!< I2C master clock frequency */

#define BH1750_SENSOR_ADDR                 0x23             /*!< slave address for BH1750 sensor */
#define BH1750_CMD_START                   0x23             /*!< Command to set measure mode */
#define ESP_SLAVE_ADDR                     0x5c// 0x38             /*!< ESP32 slave address, you can set any 7bit value */
#define WRITE_BIT                          I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                           I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                            0x0              /*!< I2C ack value */
#define NACK_VAL                           0x1              /*!< I2C nack value */

//SemaphoreHandle_t print_mux = NULL;
//;

class I2C{
  public:
    I2C();
    static esp_err_t master_read_slave(i2c_port_t i2c_num, uint8_t* data_rd, size_t size);
    static esp_err_t master_write_slave(i2c_port_t i2c_num, uint8_t* data_wr, size_t size);
 
    static esp_err_t master_sensor_test(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l);
    
    static void master_init();
    static void slave_init();
/*
    static void disp_buf(uint8_t* buf, int len);
    static void test_task(void* arg);
    static void slave_task(void* arg);
    static void master_task(void* arg);
    
    void test_start();
    void master_start();
    void slave_start();
    void master_hangs();
    void master_recovery();
    */
  public:
  uint8_t sent_pp[29];
   uint8_t rent_pp[29];
   //static SemaphoreHandle_t print_mux;
  private:
  
  private:
   
};
#endif
