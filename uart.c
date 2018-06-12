/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "driver/uart.h"

#include "freertos/queue.h"
#include "esp_log.h"
#include "soc/uart_struct.h"

#define TXD0_PIN  (GPIO_NUM_35)  //(GPIO_NUM_14)
#define RXD0_PIN  (GPIO_NUM_34)  //(GPIO_NUM_15)
#define TXD1_PIN  (GPIO_NUM_2)  //(GPIO_NUM_14)
#define RXD1_PIN  (GPIO_NUM_0)  //(GPIO_NUM_15)
#define TXD2_PIN  (GPIO_NUM_18) //(GPIO_NUM_26)
#define RXD2_PIN  (GPIO_NUM_19) //(GPIO_NUM_27)

#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE (2048)
//static void echo_task();

void uart_init() {
	uart_config_t uart_config = {
	        .baud_rate = 9600,
	        .data_bits = UART_DATA_8_BITS,
	        .parity    = UART_PARITY_DISABLE,
	        .stop_bits = UART_STOP_BITS_1,
	        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	    };

		uart_param_config(UART_NUM_0, &uart_config);
		uart_set_pin(UART_NUM_0, TXD0_PIN, RXD0_PIN, ECHO_TEST_RTS, ECHO_TEST_CTS);
		uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);uart_param_config(UART_NUM_1, &uart_config);
	    uart_set_pin(UART_NUM_1, TXD1_PIN, RXD1_PIN, ECHO_TEST_RTS, ECHO_TEST_CTS);
	    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);
	    uart_param_config(UART_NUM_2, &uart_config);
	    uart_set_pin(UART_NUM_2, TXD2_PIN, RXD2_PIN, ECHO_TEST_RTS, ECHO_TEST_CTS);
	    uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);
}

char * int2str(int x){

	static char buf[4*sizeof(x)+1];
	sprintf(buf,"%d",x);
	return buf;
	//pass will be void pass_str(char[]){printf("%s",char);}
}
int u1_send(const char *data){
    const int len=strlen(data);
    const int txBytes=uart_write_bytes(UART_NUM_1, data,len);
    return txBytes;
}

int u1_read(uint8_t *data){
    //const int len=strlen(data);
    const int rxBytes=uart_read_bytes(UART_NUM_1, data, BUF_SIZE, 20 / portTICK_RATE_MS);
    return rxBytes;
}
static void read_data(){
	uart_init();
	uint8_t *data=(uint8_t *)malloc(BUF_SIZE+1);
	int timeout=250;
	while(1) {
		//transfer u2 - >u0
		int rxBytes=uart_read_bytes(UART_NUM_2, data, BUF_SIZE, timeout/portTICK_RATE_MS);
		if(rxBytes>0){
			data[rxBytes]=0;
			printf("Received %d bytes data = %s \r\n", rxBytes,  (char *)data);
			uart_write_bytes(UART_NUM_0, (const char *) data,rxBytes );
		}
		else {
			//puts("No data received!\r\n");
		}
		//transfer u0 - >u2
		rxBytes=uart_read_bytes(UART_NUM_0, data, BUF_SIZE, timeout/portTICK_RATE_MS);
				if(rxBytes>0){
					data[rxBytes]=0;
					printf("Received %d bytes data = %s \r\n", rxBytes,  (char *)data);
					uart_write_bytes(UART_NUM_2, (const char *) data,rxBytes );
				}
				else {
					//puts("No data received!\r\n");
				}
		//vTaskDelay(300/portTICK_RATE_MS);
	}

	free(data);
}


static void echo_task() {
		uart_init();
	    // Configure a temporary buffer for the incoming data
	    int len=32;
	    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
	    char * st=(char *)malloc(len*sizeof(char)+1);

	    //char sr[len*sizeof(char)+1];
	    char * tmpc=(char *)malloc(len*sizeof(char)+1);
	    int i=0, rLen, wLen;
	    printf("Circle #=%d\n", i);
	    while (1) {
	    	//char * sr=(char *)malloc(len*sizeof(char)+1);
	    	sprintf(tmpc,"%d",i);
	    	strcpy(st,"AT");
	    	//strcat(st,tmpc);
	    	strcat(st,"\n");
	    	printf("String to be sent =%s, len=%d\n", st, (int)strlen(st));
	    	wLen=u1_send(st);
	    	printf("String len sent = %d\n", wLen);
	    	vTaskDelay(2000/portTICK_RATE_MS);

	        // Read data from the UART
	        rLen = uart_read_bytes(UART_NUM_1, data, BUF_SIZE, 20 / portTICK_RATE_MS);
	        data[rLen]=0;
	        /*if(rLen<1){
	        	sr[0]=0;
	        }
	        else{
	        	strncpy(sr,(char *)data,rLen);
	        	sr[rLen]=0;
	        }*/

	        //uart_flush(UART_NUM_1);
	        //strcat(sr,"\0");
	        //sr[rLen]="\0";
	        printf("# %d:String received=%s received length length=%d, dsp str len=%d\n", i, (char *)data, rLen, strlen((char *)data));
	        printf("String code=%#X, %#X\n", (unsigned char)data[0], (unsigned char)data[1]);
	        puts("--------------------");
	        // Write data back to the UART

	        //rLen=uart_write_bytes(UART_NUM_1, (const char *) data, len);
	        //if(rLen>0){
	        	//printf("received length = %d , buffer len=%d\r\n", rLen,len);



	        i++;
	        }


	    //free(data);

}
static void test_task(){
	while(1){
		puts("Hello Koala!");
		vTaskDelay(500/portTICK_RATE_MS);

	}
}
void app_main()
{
    //xTaskCreate(echo_task, "uart_echo_task", 3072, NULL, 10, NULL);
	//xTaskCreate(txrx_task, "txrx_task", 2048, NULL, 10, NULL);
	//xTaskCreate(test_task, "test_task", 3072, NULL, 10, NULL);
	xTaskCreate(read_data, "read_data", 3072, NULL, 10, NULL);
}

