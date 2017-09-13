/* Very basic example that just demonstrates we can run at all!
 */
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
	
	int pinA = 1;
	int pinB = 3;
	int pinC = 15;
	int pinD = 13;
	int pinE = 12;
	int pinF = 14;
	int pinG = 2;

void seset7()
{
  gpio_write(pinA, 0);
  gpio_write(pinB, 0);
  gpio_write(pinC, 0);
  gpio_write(pinD, 0);
  gpio_write(pinE, 0);
  gpio_write(pinF, 0);
  gpio_write(pinG, 0);
  //gpio_write(pinDP, 0);
}

void set7 (int num){

	switch(num) {		
		case 1:
			seset7();
  			gpio_write(pinB, 1);
  			gpio_write(pinC, 1);
  		break;
  		case 2:
  			seset7();
  			gpio_write(pinA, 1);
		   gpio_write(pinB, 1);
  			gpio_write(pinD, 1);
  			gpio_write(pinE, 1);
  			gpio_write(pinG, 1);
  		break;
  		case 3:
  			seset7();
  			gpio_write(pinA, 1);
  			gpio_write(pinB, 1);
  			gpio_write(pinC, 1);
  			gpio_write(pinD, 1);
  			gpio_write(pinG, 1);
  		break;
  		case 4:
  			seset7();
  			gpio_write(pinA, 1);
  			gpio_write(pinB, 1);
  			gpio_write(pinC, 1);
  			gpio_write(pinD, 1);
  			gpio_write(pinG, 1);
  		break;
  		case 5:
  			seset7();
  			gpio_write(pinA, 1);
  			gpio_write(pinC, 1);
  			gpio_write(pinD, 1);
  			gpio_write(pinF, 1);
  			gpio_write(pinG, 1);
  		break;
  		case 6:
  			seset7();
  			gpio_write(pinA, 1);
  			gpio_write(pinC, 1);
  			gpio_write(pinD, 1);
  			gpio_write(pinE, 1);
  			gpio_write(pinF, 1);
  			gpio_write(pinG, 1);
  		break;
  		case 7:
  			seset7();
  			gpio_write(pinA, 1);
  			gpio_write(pinB, 1);
  			gpio_write(pinC, 1);
  		break;
  		case 8:
  			seset7();
		  	gpio_write(pinA, 1);
        	gpio_write(pinB, 1);
			gpio_write(pinC, 1);
			gpio_write(pinD, 1);
			gpio_write(pinE, 1);
			gpio_write(pinF, 1);
			gpio_write(pinG, 1); 		
  		break;
  		case 9:
  			seset7();
		  	gpio_write(pinA, 1);
			gpio_write(pinB, 1);
			gpio_write(pinC, 1);
			gpio_write(pinD, 1);
			gpio_write(pinF, 1);
			gpio_write(pinG, 1);		
  		break;
  		case 0:
  		  seset7();
  		  gpio_write(pinA, 1);
		  gpio_write(pinB, 1);
		  gpio_write(pinC, 1);
		  gpio_write(pinD, 1);
		  gpio_write(pinE, 1);
		  gpio_write(pinF, 1);
  		break;
		
	}


}

void task1(void *pvParameters)
{
    QueueHandle_t *queue = (QueueHandle_t *)pvParameters;
    printf("Hello from task1!\r\n");
    gpio_enable(12, GPIO_OUTPUT);
    while(1) {
	
	printf("1!\r\n");
	gpio_write(12, 1);
	vTaskDelay(200 / portTICK_PERIOD_MS);
	gpio_write(12, 0);
	vTaskDelay(200 / portTICK_PERIOD_MS);
    
    }
}

void task2(void *pvParameters)
{
    printf("Hello from task 2!\r\n");
    gpio_enable(13, GPIO_OUTPUT);
    QueueHandle_t *queue = (QueueHandle_t *)pvParameters;
    while(1) {

	printf("2!\r\n");
	gpio_write(13, 1);
	vTaskDelay(200 / portTICK_PERIOD_MS);
	gpio_write(13, 0);
	vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
void task3(void *pvParameters)
{
    printf("Hello from task 3!\r\n");
    QueueHandle_t *queue = (QueueHandle_t *)pvParameters;
    gpio_enable(14, GPIO_OUTPUT);
    while(1) {
	
	printf("3!\r\n");
	gpio_write(14, 1);
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	gpio_write(14, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static QueueHandle_t mainqueue;

void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());
    mainqueue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(task1, "tsk1", 256, &mainqueue, 2, NULL);
    xTaskCreate(task2, "tsk2", 256, &mainqueue, 2, NULL);
    xTaskCreate(task3, "tsk3", 256, &mainqueue, 2, NULL);

}
