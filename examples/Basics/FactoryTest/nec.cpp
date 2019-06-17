#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/periph_ctrl.h"

#include "freertos/queue.h"
#include "nvs_flash.h"

#include "esp32_rmt.h"

#define NEC_HEADER_HIGH_US    9000                         /*!< NEC protocol header: positive 9ms */
#define NEC_HEADER_LOW_US     4500                         /*!< NEC protocol header: negative 4.5ms*/
#define NEC_BIT_ONE_HIGH_US    560                         /*!< NEC protocol data bit 1: positive 0.56ms */
#define NEC_BIT_ONE_LOW_US    (2250-NEC_BIT_ONE_HIGH_US)   /*!< NEC protocol data bit 1: negative 1.69ms */
#define NEC_BIT_ZERO_HIGH_US   560                         /*!< NEC protocol data bit 0: positive 0.56ms */
#define NEC_BIT_ZERO_LOW_US   (1120-NEC_BIT_ZERO_HIGH_US)  /*!< NEC protocol data bit 0: negative 0.56ms */
#define NEC_BIT_END            560                         /*!< NEC protocol end: positive 0.56ms */
#define NEC_BIT_MARGIN         150                          /*!< NEC parse margin time */

#define NEC_ITEM_DURATION(d)  ((d & 0x7fff)*10/RMT_TICK_10_US)  /*!< Parse duration time from memory register value */
#define NEC_DATA_ITEM_NUM   34  /*!< NEC code item number: header + 32bit data + end */

#define rmt_item32_tIMEOUT_US  9500   /*!< RMT receiver timeout value(us) */

#define RMT_TX_CARRIER_EN 1

#define NEC_FREQ 38000

//send 1 frame of NEC data.

void ESP32_RMT::necSend(uint16_t addr, uint16_t cmd)
{
	rmt_config_t rmt_tx;
	rmt_tx.channel = (rmt_channel_t)txChannel;
	rmt_tx.gpio_num =(gpio_num_t) txPin;
	rmt_tx.mem_block_num = 1;
	rmt_tx.clk_div = RMT_CLK_DIV;
	rmt_tx.tx_config.loop_en = false;
	rmt_tx.tx_config.carrier_duty_percent = 50;
	rmt_tx.tx_config.carrier_freq_hz = NEC_FREQ;
	rmt_tx.tx_config.carrier_level = (rmt_carrier_level_t)1;
	rmt_tx.tx_config.carrier_en = RMT_TX_CARRIER_EN;
	rmt_tx.tx_config.idle_level = (rmt_idle_level_t)0;
	rmt_tx.tx_config.idle_output_en = true;
	rmt_tx.rmt_mode = (rmt_mode_t)0;
	rmt_config(&rmt_tx);
	rmt_driver_install(rmt_tx.channel, 0, RMT_INTR_NUM);
	
	//printf("cmd: %d, data: %d \n\r ", cmd, addr);
    int channel = RMT_TX_CHANNEL;
    int nec_tx_num = RMT_TX_DATA_NUM;
  
      //  ESP_LOGI(NEC_TAG, "RMT TX DATA");
        size_t size = (sizeof(rmt_item32_t) * NEC_DATA_ITEM_NUM * nec_tx_num);
        //each item represent a cycle of waveform.
        rmt_item32_t* item = (rmt_item32_t*) malloc(size);
        int item_num = NEC_DATA_ITEM_NUM * nec_tx_num;
		//printf("item_num:%d \n\r", item_num);
        memset((void*) item, 0, size);
        int i, offset = 0;
        while(1) {
            //To build a series of waveforms.
            i = nec_build_items((rmt_channel_t)channel, item + offset, item_num - offset,  addr, cmd);
            if(i < 0) {
                break;
            }
            cmd++;
            addr++;
            offset += i;
        }
        //To send data according to the waveform items.
        rmt_write_items((rmt_channel_t)channel, item, item_num, true);
        //Wait until sending is done.
        rmt_wait_tx_done((rmt_channel_t)channel);
        //before we free the data, make sure sending is already done.
        free(item);
    
}

/*
 * @brief Build register value of waveform for NEC one data bit
 */
inline void ESP32_RMT::nec_fill_item_level(rmt_item32_t* item, int high_us, int low_us)
{
    item->level0 = 1;
    item->duration0 = (high_us) / 10 * RMT_TICK_10_US;
    item->level1 = 0;
    item->duration1 = (low_us) / 10 * RMT_TICK_10_US;
}

/*
 * @brief Generate NEC header value: active 9ms + negative 4.5ms
 */
void ESP32_RMT::nec_fill_item_header(rmt_item32_t* item)
{
    nec_fill_item_level(item, NEC_HEADER_HIGH_US, NEC_HEADER_LOW_US);
}

/*
 * @brief Generate NEC data bit 1: positive 0.56ms + negative 1.69ms
 */
 void ESP32_RMT::nec_fill_item_bit_one(rmt_item32_t* item)
{
    nec_fill_item_level(item, NEC_BIT_ONE_HIGH_US, NEC_BIT_ONE_LOW_US);
}

/*
 * @brief Generate NEC data bit 0: positive 0.56ms + negative 0.56ms
 */
void ESP32_RMT::nec_fill_item_bit_zero(rmt_item32_t* item)
{
    nec_fill_item_level(item, NEC_BIT_ZERO_HIGH_US, NEC_BIT_ZERO_LOW_US);
}

/*
 * @brief Generate NEC end signal: positive 0.56ms
 */
void ESP32_RMT::nec_fill_item_end(rmt_item32_t* item)
{
    nec_fill_item_level(item, NEC_BIT_END, 0x7fff);
}


int ESP32_RMT::nec_build_items(int channel, rmt_item32_t* item, int item_num, uint16_t addr, uint16_t cmd_data)
{
    int i = 0, j = 0;
    if(item_num < NEC_DATA_ITEM_NUM) {
        return -1;
    }
    nec_fill_item_header(item++);
    i++;
    for(j = 0; j < 16; j++) {
        if(addr & 0x1) {
            nec_fill_item_bit_one(item);
        } else {
            nec_fill_item_bit_zero(item);
        }
        item++;
        i++;
        addr >>= 1;
    }
    for(j = 0; j < 16; j++) {
        if(cmd_data & 0x1) {
            nec_fill_item_bit_one(item);
        } else {
            nec_fill_item_bit_zero(item);
        }
        item++;
        i++;
        cmd_data >>= 1;
    }
    nec_fill_item_end(item);
    i++;
    return i;
}
