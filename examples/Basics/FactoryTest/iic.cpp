#include"iic.h"

SemaphoreHandle_t print_mux = NULL;
xTaskHandle master_xHandle;
I2C::I2C(void)
{
  //print_mux = NULL;
   for(int i = 0; i < 9 ; i++){
    rent_pp[i] = 0;
   }
}


esp_err_t I2C::master_read_slave(i2c_port_t i2c_num, uint8_t* data_rd, size_t size)
{
    if (size == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ( ESP_SLAVE_ADDR << 1 ) | READ_BIT, ACK_CHECK_EN);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, I2C_MASTER_ACK); //ACK_VAL
    }
    i2c_master_read_byte(cmd, data_rd + size - 1,I2C_MASTER_NACK); //NACK_VAL
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t I2C::master_write_slave(i2c_port_t i2c_num, uint8_t* data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ( ESP_SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t I2C::master_sensor_test(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BH1750_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, BH1750_CMD_START, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        return ret;
    }
    vTaskDelay(30 / portTICK_RATE_MS);
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BH1750_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
    //i2c_master_read_byte(cmd, data_h, ACK_VAL);
    //i2c_master_read_byte(cmd, data_l, NACK_VAL);
    
    i2c_master_read_byte(cmd, data_h, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, data_l, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

void I2C::master_init()
{
    i2c_port_t i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                       I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
}

void I2C::slave_init()
{
    i2c_port_t i2c_slave_port = I2C_EXAMPLE_SLAVE_NUM;
    i2c_config_t conf_slave;
    conf_slave.sda_io_num = I2C_EXAMPLE_SLAVE_SDA_IO;
    conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.scl_io_num = I2C_EXAMPLE_SLAVE_SCL_IO;
    conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.mode = I2C_MODE_SLAVE;
    conf_slave.slave.addr_10bit_en = 0;
    conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;
    i2c_param_config(i2c_slave_port, &conf_slave);
    i2c_driver_install(i2c_slave_port, conf_slave.mode,
                       I2C_EXAMPLE_SLAVE_RX_BUF_LEN,
                       I2C_EXAMPLE_SLAVE_TX_BUF_LEN, 0);
}
#if 0
void I2C::disp_buf(uint8_t* buf, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
        if (( i + 1 ) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

void I2C::test_task(void* arg)
{
    int i = 0;
    int ret;
    uint32_t task_idx = (uint32_t) arg;
    uint8_t* data = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_wr = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_rd = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t sensor_data_h, sensor_data_l;
    int cnt = 0;
    while (1) {
        printf("test cnt: %d\n", cnt++);
        ret = master_sensor_test(I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        xSemaphoreTake(print_mux, portMAX_DELAY);
        if(ret == ESP_ERR_TIMEOUT) {
            printf("I2C timeout\n");
        } else if(ret == ESP_OK) {
            printf("*******************\n");
            printf("TASK[%d]  MASTER READ SENSOR( BH1750 )\n", task_idx);
            printf("*******************\n");
            printf("data_h: %02x\n", sensor_data_h);
            printf("data_l: %02x\n", sensor_data_l);
            printf("sensor val: %f\n", (sensor_data_h << 8 | sensor_data_l) / 1.2);
        } else {
            printf("%s: No ack, sensor not connected...skip...\n", esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux);
        vTaskDelay(( DELAY_TIME_BETWEEN_ITEMS_MS * ( task_idx + 1 ) ) / portTICK_RATE_MS);
        //---------------------------------------------------
        for (i = 0; i < DATA_LENGTH; i++) {
            data[i] = i;
        }
        xSemaphoreTake(print_mux, portMAX_DELAY);
        size_t d_size = i2c_slave_write_buffer(I2C_EXAMPLE_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
        if (d_size == 0) {
            printf("i2c slave tx buffer full\n");
            ret = master_read_slave(I2C_EXAMPLE_MASTER_NUM, data_rd, DATA_LENGTH);
        } else {
            ret = master_read_slave(I2C_EXAMPLE_MASTER_NUM, data_rd, RW_TEST_LENGTH);
        }

        if (ret == ESP_ERR_TIMEOUT) {
            printf("I2C timeout\n");
            printf("*********\n");
        } else if (ret == ESP_OK) {
            printf("*******************\n");
            printf("TASK[%d]  MASTER READ FROM SLAVE\n", task_idx);
            printf("*******************\n");
            printf("====TASK[%d] Slave buffer data ====\n", task_idx);
            disp_buf(data, d_size);
            printf("====TASK[%d] Master read ====\n", task_idx);
            disp_buf(data_rd, d_size);
        } else {
            printf("%s: Master read slave error, IO not connected...\n", esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux);
        vTaskDelay(( DELAY_TIME_BETWEEN_ITEMS_MS * ( task_idx + 1 ) ) / portTICK_RATE_MS);
        //---------------------------------------------------
        int size;
        for (i = 0; i < DATA_LENGTH; i++) {
            data_wr[i] = i + 10;
        }
        xSemaphoreTake(print_mux, portMAX_DELAY);
        //we need to fill the slave buffer so that master can read later
        ret = master_write_slave( I2C_EXAMPLE_MASTER_NUM, data_wr, RW_TEST_LENGTH);
        if (ret == ESP_OK) {
            size = i2c_slave_read_buffer( I2C_EXAMPLE_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
        }
        if (ret == ESP_ERR_TIMEOUT) {
            printf("I2C timeout\n");
        } else if (ret == ESP_OK) {
            printf("*******************\n");
            printf("TASK[%d]  MASTER WRITE TO SLAVE\n", task_idx);
            printf("*******************\n");
            printf("----TASK[%d] Master write ----\n", task_idx);
            disp_buf(data_wr, RW_TEST_LENGTH);
            printf("----TASK[%d] Slave read: [%d] bytes ----\n", task_idx, size);
            disp_buf(data, size);
        } else {
            printf("TASK[%d] %s: Master write slave error, IO not connected....\n", task_idx, esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux);
        vTaskDelay(( DELAY_TIME_BETWEEN_ITEMS_MS * ( task_idx + 1 ) ) / portTICK_RATE_MS);
    }
}

/*
esp_err_t i2c_reset_tx_fifo(i2c_port_t i2c_num);
esp_err_t i2c_reset_rx_fifo(i2c_port_t i2c_num);
*/
void I2C::slave_task(void* arg)
{
    int i = 0;
    int ret;
    uint32_t task_idx = (uint32_t) arg;
    uint8_t* data = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_wr = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_rd = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t sensor_data_h, sensor_data_l;
    int cnt = 0;
    while (true) {
        printf("test cnt: %d\n", cnt++);
        //---------------------------------------------------
        for (i = 0; i < 9; i++) {
            data[i] = sent_pp[i];
            //data[i] = i;
        }
        //sent_pp[i] = pp[i]*100/(fb->len/18);
        xSemaphoreTake(print_mux, portMAX_DELAY);
        size_t d_size = i2c_slave_write_buffer(I2C_EXAMPLE_SLAVE_NUM, data, 9, 100 / portTICK_RATE_MS);
       
        if (d_size == 0) {
            printf("i2c slave tx buffer full\n");
            //ret = master_read_slave(I2C_EXAMPLE_MASTER_NUM, data_rd, DATA_LENGTH);
        } else {
            //ret = master_read_slave(I2C_EXAMPLE_MASTER_NUM, data_rd, RW_TEST_LENGTH);
            printf("d_size = %d\n",d_size);
        }
        xSemaphoreGive(print_mux);
        vTaskDelay(( DELAY_TIME_BETWEEN_ITEMS_MS * ( task_idx + 1 ) ) / portTICK_RATE_MS);
        
        //---------------------------------------------------
        /*
        int size;
        for (i = 0; i < DATA_LENGTH; i++) {
            data_wr[i] = i + 10;
        }
        xSemaphoreTake(print_mux, portMAX_DELAY);
        //we need to fill the slave buffer so that master can read later
        //ret = master_write_slave( I2C_EXAMPLE_MASTER_NUM, data_wr, RW_TEST_LENGTH);
       
       // if (ret == ESP_OK) {
            size = i2c_slave_read_buffer( I2C_EXAMPLE_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
            printf("size %d\n",size);
       // }
        if (ret == ESP_ERR_TIMEOUT) {
            printf("I2C timeout\n");
        } else if (ret == ESP_OK) {
            printf("*******************\n");
            printf("TASK[%d]  MASTER WRITE TO SLAVE\n", task_idx);
            printf("*******************\n");
            printf("----TASK[%d] Master write ----\n", task_idx);
            disp_buf(data_wr, RW_TEST_LENGTH);
            printf("----TASK[%d] Slave read: [%d] bytes ----\n", task_idx, size);
            disp_buf(data, size);
        } else {
            printf("TASK[%d] %s: Master write slave error, IO not connected....\n", task_idx, esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux);
        vTaskDelay(( DELAY_TIME_BETWEEN_ITEMS_MS * ( task_idx + 1 ) ) / portTICK_RATE_MS);
        */
    }
}


void I2C::master_task(void* arg)
{
    int i = 0;
    int ret;
    uint32_t task_idx = (uint32_t) arg;
    uint8_t* data = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_wr = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_rd = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t sensor_data_h, sensor_data_l;
    int cnt = 0;
    while(true) {
        printf("test cnt: %d\n", cnt++);
        xSemaphoreTake(print_mux, portMAX_DELAY);
        ret = master_read_slave(I2C_EXAMPLE_MASTER_NUM, data_rd, DATA_LENGTH);
        if (ret == ESP_ERR_TIMEOUT) {
            printf("I2C timeout\n");
            printf("*********\n");
        } else if (ret == ESP_OK) {
          #if 0
            printf("*******************\n");
            printf("TASK[%d]  MASTER READ FROM SLAVE\n", task_idx);
            printf("*******************\n");
            printf("====TASK[%d] Slave buffer data ====\n", task_idx);
            //disp_buf(data, RW_TEST_LENGTH);
            printf("====TASK[%d] Master read ====\n", task_idx);
            disp_buf(data_rd, 29);
           #else
             for(int i = 0; i < 29; i++){  
               rent_pp[i] = data_rd[i];
             }
           #endif
        } else {
            printf("%s: Master read slave error, IO not connected...\n", esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux);
        //vTaskDelay(( DELAY_TIME_BETWEEN_ITEMS_MS * ( task_idx + 1 ) ) / portTICK_RATE_MS);
        vTaskDelay(2 / portTICK_RATE_MS);
        
        //---------------------------------------------------
        /*
        int size;
        for (i = 0; i < DATA_LENGTH; i++) {
            data_wr[i] = i;
        }
        xSemaphoreTake(print_mux, portMAX_DELAY);
        //we need to fill the slave buffer so that master can read later
        ret = master_write_slave( I2C_EXAMPLE_MASTER_NUM, data_wr, RW_TEST_LENGTH);
       
        xSemaphoreGive(print_mux);
        vTaskDelay(( DELAY_TIME_BETWEEN_ITEMS_MS * ( task_idx + 1 ) ) / portTICK_RATE_MS);
        */
    }
}

void I2C::test_start(){
  print_mux = xSemaphoreCreateMutex();
  slave_init();
  master_init();
  xTaskCreate(test_task, "i2c_test_task_0", 1024 * 2, (void* ) 0, 10, NULL);
  xTaskCreate(test_task, "i2c_test_task_1", 1024 * 2, (void* ) 1, 10, NULL);
}

void I2C::master_start(){
  print_mux = xSemaphoreCreateMutex();
  master_init();
  xTaskCreate(master_task, "master_task", 1024 * 2, (void* ) 0, 10, &master_xHandle);
}


void I2C::master_hangs(){
  //print_mux = xSemaphoreCreateMutex();
  //master_init();
  //xTaskCreate(master_task, "master_task", 1024 * 2, (void* ) 0, 10, &master_xHandle);
  vTaskSuspend(master_xHandle); 
}
void I2C::master_recovery(){
  //print_mux = xSemaphoreCreateMutex();
  //master_init();
  //xTaskCreate(master_task, "master_task", 1024 * 2, (void* ) 0, 10, &master_xHandle);
   vTaskResume(master_xHandle); 
}
void I2C::slave_start(){
  print_mux = xSemaphoreCreateMutex();
  slave_init();
  xTaskCreate(slave_task, "slave_task", 1024 * 2, (void* ) 0, 10, NULL);
}
#endif