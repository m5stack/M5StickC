#include <stdio.h>
#include <string.h>

#include "esp32_rmt.h"

#define TRANSMIT 1
#define RECEIVE 0

#if RMT_RX_SELF_TEST
#define RMT_RX_ACTIVE_LEVEL  0   /*!< Data bit is active high for self test mode */
#define RMT_TX_CARRIER_EN    0   /*!< Disable carrier for self test mode  */
#else
//Test with infrared LED, we have to enable carrier for transmitter
//When testing via IR led, the receiver waveform is usually active-low.
#define RMT_RX_ACTIVE_LEVEL  0   /*!< If we connect with a IR receiver, the data is active low */
#define RMT_TX_CARRIER_EN    1   /*!< Enable carrier for IR transmitter test with IR led */
#endif


ESP32_RMT::ESP32_RMT()
{
	//printf("booting remote peripheral\n\r");
	//printf("hello world\n\r");
}

void ESP32_RMT::begin(uint8_t pin, bool mode)
{
	if(mode==TRANSMIT)
	{

		txPin = pin;	
	
	}	
}





void ESP32_RMT::rmt_tx_init()
{
  
}




void ESP32_RMT::send(uint8_t data)
{




    int channel =  (rmt_channel_t)RMT_TX_CHANNEL;
   
    int nec_tx_num = RMT_TX_DATA_NUM;
    for(;;) {
		//ESP_LOGI(NEC_TAG, "RMT TX DATA");
        size_t size = (sizeof(rmt_item32_t) * 1);
        //each item represent a cycle of waveform.
        rmt_item32_t* item = (rmt_item32_t*) malloc(size);
			
		item->level0 = 1;
		item->duration0 = (9000) / 10 * RMT_TICK_10_US;
		item->level1 = 0;
		item->duration1 = (4500) / 10 * RMT_TICK_10_US;
		
		
		//printf("writing items\n\r");
        rmt_write_items((rmt_channel_t)channel, item, 2, true);
        //Wait until sending is done.
        //rmt_wait_tx_done((rmt_channel_t)channel);
        //before we free the data, make sure sending is already done.
        free(item);
    }
	
}
