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
#include "driver/i2c.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "soc/uart_struct.h"
//UART
#define TXD0_PIN  (GPIO_NUM_35)  //(GPIO_NUM_14)
#define RXD0_PIN  (GPIO_NUM_34)  //(GPIO_NUM_15)
#define TXD1_PIN  (GPIO_NUM_2)  //(GPIO_NUM_14)
#define RXD1_PIN  (GPIO_NUM_0)  //(GPIO_NUM_15)
#define TXD2_PIN  (GPIO_NUM_18) //(GPIO_NUM_26)
#define RXD2_PIN  (GPIO_NUM_19) //(GPIO_NUM_27)

#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE (2048)
//end of UART
//I2C
#define DATA_LENGTH                        512              /*!<Data buffer length for test buffer*/
#define RW_TEST_LENGTH                     129              /*!<Data length for r/w test, any value from 0-DATA_LENGTH*/
#define DELAY_TIME_BETWEEN_ITEMS_MS        1234             /*!< delay time between different test items */


#define I2C_SCL_IO          22               /*!< gpio number for I2C master clock */
#define I2C_SDA_IO          23               /*!< gpio number for I2C master data  */
#define I2C_NUM             I2C_NUM_0        /*!< I2C port number for master dev */
#define I2C_TX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_RX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_FREQ_HZ         100000           /*!< I2C master clock frequency */

#define LHT_SNR_ADDR                 0x45             /*!< slave address for BH1750 sensor */
#define LHT_SNR_START                0x45             /*!< Command to set measure mode */
#define LHT_MFC_REG                  0x7E
#define LHT_PID_REG                  0x7F
#define LHT_CTR_REG                  0x01
#define LHT_RLT_REG                  0x00
#define LHT_L_REG                    0x02
#define LHT_H_REG                    0x03
#define ESP_SLAVE_ADDR                     0x28             /*!< ESP32 slave address, you can set any 7bit value */
#define WRITE_BIT                          I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                           I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                            0x0              /*!< I2C ack value */
#define NACK_VAL 						   0x1 				/*!< I2C nack value */

SemaphoreHandle_t print_mux = NULL;
//end of I2C

void uart_init(int brate) {
	uart_config_t uart_config = {
	        .baud_rate = brate,
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

static void echo_task() {
		uart_init(115200);
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

static void u02(){
	uart_init(9600);
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
static void u01(){
	uart_init(115200);
	uint8_t *data=(uint8_t *)malloc(BUF_SIZE+1);
	int timeout=250;
	while(1) {
		//transfer u2 - >u0
		int rxBytes=uart_read_bytes(UART_NUM_1, data, BUF_SIZE, timeout/portTICK_RATE_MS);
		if(rxBytes>0){
			data[rxBytes]=0;
			printf("ESP Received %d bytes data = %s \r\n", rxBytes,  (char *)data);
			uart_write_bytes(UART_NUM_0, (const char *) data,rxBytes );
		}
		else {
			//puts("No data received!\r\n");
		}
		//transfer u0 - >u2
		rxBytes=uart_read_bytes(UART_NUM_0, data, BUF_SIZE, timeout/portTICK_RATE_MS);
				if(rxBytes>0){
					data[rxBytes]=0;
					printf("\n USB Received %d bytes data = %s \r\n", rxBytes,  (char *)data);
					uart_write_bytes(UART_NUM_1, (const char *) data,rxBytes );

				}
				else {
					//puts("No data received!\r\n");
				}
		//vTaskDelay(300/portTICK_RATE_MS);
	}

	free(data);
}

static void i2c_init()
{
    puts("I2C init! \n");
	int ret;
	int i2c_master_port = I2C_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_FREQ_HZ;
    ret=i2c_param_config(i2c_master_port, &conf);
    printf("I2C par status=%d\n", ret);
    ret=i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_RX_BUF_DISABLE,
                       I2C_TX_BUF_DISABLE, 0);
    printf("I2C driver status=%d\n", ret);
}
/**
 * @brief test code to read esp-i2c-slave
 *        We need to fill the buffer of esp slave device, then master can read them out.
 *
 * _______________________________________________________________________________________
 * | start | slave_addr + rd_bit +ack | read n-1 bytes + ack | read 1 byte + nack | stop |
 * --------|--------------------------|----------------------|--------------------|------|
 *
 */
static esp_err_t i2c_read(i2c_port_t i2c_num, uint8_t* data_rd, size_t size)
{
    if (size == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ( LHT_SNR_ADDR << 1 ) | READ_BIT, ACK_CHECK_EN);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief Test code to write esp-i2c-slave
 *        Master device write data to slave(both esp32),
 *        the data will be stored in slave buffer.
 *        We can read them out from slave buffer.
 *
 * ___________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------------|------|
 *
 */
static esp_err_t i2c_write(i2c_port_t i2c_num, uint8_t* data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ( LHT_SNR_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}
static void i2c_scan() {
	vTaskDelay(3000 / portTICK_RATE_MS);
	puts("Scan start!\n");
	i2c_init();
	printf("scanning the bus...\r\n\r\n");
	int devices_found = 0;

	for(int address = 1; address < 127; address++) {

		// create and execute the command link
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
		i2c_master_stop(cmd);
		if(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS) == ESP_OK) {
			printf("-> found device with address 0x%02x\r\n", address);
			devices_found++;
		}
		i2c_cmd_link_delete(cmd);
	}
	if(devices_found == 0) printf("\r\n-> no devices found\r\n");
	printf("\r\n...scan completed!\r\n");
}
/* @brief test function to show buffer
*/
static void disp_buf(uint8_t* buf, int len)
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

/**
 * @brief test code to write esp-i2c-slave
 *
 * 1. set mode
 * _________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write 1 byte + ack  | stop |
 * --------|---------------------------|---------------------|------|
 * 2. wait more than 24 ms
 * 3. read data
 * ______________________________________________________________________________________
 * | start | slave_addr + rd_bit + ack | read 1 byte + ack  | read 1 byte + nack | stop |
 * --------|---------------------------|--------------------|--------------------|------|
 */
static esp_err_t i2c_sensor_test(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, LHT_SNR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, LHT_MFC_REG, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        return ret;
    }
    vTaskDelay(30 / portTICK_RATE_MS);
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, LHT_SNR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data_h, ACK_VAL);
    i2c_master_read_byte(cmd, data_l, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

static void i2c_test_task(void* arg) {
	vTaskDelay(3000 / portTICK_RATE_MS);
	puts("Starts!\n");
	i2c_init();

	int ret;
    uint32_t task_idx = (uint32_t) arg;
    uint8_t* data = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_wr = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_rd = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t sensor_data_h, sensor_data_l;
    int cnt = 0;
    puts("Set control to continues");
	data_wr[0]=LHT_CTR_REG;
	data_wr[1]=0xCE;
	data_wr[2]=0x10;
	ret=i2c_write(I2C_NUM, data_wr, 3);
	if (ret != ESP_OK) {
		puts("Write Error!\n");
		printf("I2C write status=%d\n", ret);
	}
	else {
		puts("Control Write Successfully!\n");
		vTaskDelay(30 / portTICK_RATE_MS);


	}

    while (1) {
        printf("Read OPT3001 ID, cnt: %d\n", cnt++);
        data_wr[0]=LHT_RLT_REG;
        ret=i2c_write(I2C_NUM, data_wr, 1);
        if (ret != ESP_OK) {
            puts("Write Error!\n");
            //printf("I2C write status=%d\n", ret);
        }
        else {
        	puts("Write Result Reg Successfully!\n");
        	vTaskDelay(30 / portTICK_RATE_MS);
        	ret=i2c_read(I2C_NUM, data_rd, 2);
        	if(ret != ESP_OK) {
        		puts("Read Result Error!\n");
        	}
        	else {
        		puts("Read Result Successfully!\n");
        		disp_buf(data_rd,2);
        	}

        }
        vTaskDelay(500 / portTICK_RATE_MS);
    }

}

static void test_task(){
	while(1){
		puts("Hello Koala!");
		vTaskDelay(500/portTICK_RATE_MS);

	}
}
// loop task
void loop_task(
		)
{
    while(1) {
		vTaskDelay(5000 / portTICK_RATE_MS);
    }
}
void app_main()
{

	//xTaskCreate(echo_task, "uart_echo_task", 3072, NULL, 10, NULL);
	//xTaskCreate(txrx_task, "txrx_task", 2048, NULL, 10, NULL);
	//xTaskCreate(test_task, "test_task", 3072, NULL, 10, NULL);
	//xTaskCreate(u02, "GPS", 3072, NULL, 10, NULL);
	//xTaskCreate(u01, "LTE", 3072, NULL, 10, NULL);
	//xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 2, (void* ) 0, 10, NULL);
	i2c_scan();
	//xTaskCreate(loop_task, "loop_task", 2048, NULL, 5, NULL);
}
