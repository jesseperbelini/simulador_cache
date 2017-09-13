/* Desligar o display de 7 segmentos antes de gravar
   pois o mesmo usa as portas de gravação
 */
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "esp8266.h"
    //pinos do display
    const int pinA = 1;
    const int pinB = 3;
    const int pinC = 15;
    const int pinD = 13;
    const int pinE = 12;
    const int pinF = 14;
    const int pinG = 2;
    //pinos dos farois
    const int verde = 4;
    const int amarelo = 5;
    const int vermelho = 16;

    int flag = 0;

    /* pin config */
    const int gpio = 0;   /* gpio 0 usually has "PROGRAM" button attached */
    const int active = 0; /* active == 0 for active low */
    const gpio_inttype_t int_type = GPIO_INTTYPE_EDGE_NEG;
 
void reset7() {
  gpio_write(pinA, 1);
  gpio_write(pinB, 1);
  gpio_write(pinC, 1);
  gpio_write(pinD, 1);
  gpio_write(pinE, 1);
  gpio_write(pinF, 1);
  gpio_write(pinG, 1);
  //gpio_write(pinDP, 1);
}
 
void set7 (int num) {
 
    switch(num) {       
        case 1:
            gpio_write(pinA, 1);
            gpio_write(pinB, 0);
            gpio_write(pinC, 0);
            gpio_write(pinD, 1);
            gpio_write(pinE, 1);
            gpio_write(pinF, 1);
            gpio_write(pinG, 1);  
        break;
        case 2:
            gpio_write(pinA, 0);
            gpio_write(pinB, 0);
            gpio_write(pinC, 1);
            gpio_write(pinD, 0);
            gpio_write(pinE, 0);
            gpio_write(pinF, 1);
            gpio_write(pinG, 0);  
        break;
        case 3:
            gpio_write(pinA, 0);
            gpio_write(pinB, 0);
            gpio_write(pinC, 0);
            gpio_write(pinD, 0);
            gpio_write(pinE, 1);
            gpio_write(pinF, 1);
            gpio_write(pinG, 0);  
        break;
        case 4:
            gpio_write(pinA, 1);
            gpio_write(pinB, 0);
            gpio_write(pinC, 0);
            gpio_write(pinD, 1);
            gpio_write(pinE, 1);
            gpio_write(pinF, 0);
            gpio_write(pinG, 0);  
        break;
        case 5:
            gpio_write(pinA, 0);
            gpio_write(pinB, 1);
            gpio_write(pinC, 0);
            gpio_write(pinD, 0);
            gpio_write(pinE, 1);
            gpio_write(pinF, 0);
            gpio_write(pinG, 0);  
        break;
        case 6:
            gpio_write(pinA, 0);
            gpio_write(pinB, 1);
            gpio_write(pinC, 0);
            gpio_write(pinD, 0);
            gpio_write(pinE, 0);
            gpio_write(pinF, 0);
            gpio_write(pinG, 0);  
        break;
        case 7:
            gpio_write(pinA, 0);
            gpio_write(pinB, 0);
            gpio_write(pinC, 0);
            gpio_write(pinD, 1);
            gpio_write(pinE, 1);
            gpio_write(pinF, 1);
            gpio_write(pinG, 1);  
        break;
        case 8:
            gpio_write(pinA, 0);
            gpio_write(pinB, 0);
            gpio_write(pinC, 0);
            gpio_write(pinD, 0);
            gpio_write(pinE, 0);
            gpio_write(pinF, 0);
            gpio_write(pinG, 0);        
        break;
        case 9:
            gpio_write(pinA, 0);
            gpio_write(pinB, 0);
            gpio_write(pinC, 0);
            gpio_write(pinD, 0);
            gpio_write(pinE, 1);
            gpio_write(pinF, 0);
            gpio_write(pinG, 0);          
        break;
        case 0:
            gpio_write(pinA, 0);
            gpio_write(pinB, 0);
            gpio_write(pinC, 0);
            gpio_write(pinD, 0);
            gpio_write(pinE, 0);
            gpio_write(pinF, 0);
            gpio_write(pinG, 1);
        break;
         
    }
 
 
}
 
void farol(void *pvParameters) {
    
    QueueHandle_t *queue = (QueueHandle_t *)pvParameters;
    while(1) {
        gpio_write(verde, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_write(verde, 1);
         gpio_write(amarelo, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_write(amarelo, 1);
           gpio_write(vermelho, 0);
            if (flag==1){ //verifica bandeira
            //vTaskDelay(1000 / portTICK_PERIOD_MS);
            cronometro();
            reset7();
            flag=0; //libera bandeira
        } 
        vTaskDelay(1000 / portTICK_PERIOD_MS); 
        gpio_write(vermelho, 1);
 
    }
}

void cronometro() {
        int num;
        //vTaskDelay(5000 / portTICK_PERIOD_MS);
        for(num=9;num>=0;num--){
           set7(num);
           vTaskDelay(200 / portTICK_PERIOD_MS);
        } 
        printf("teste");   
}

void gpio_intr_handler(uint8_t gpio_num);

/* This task configures the GPIO interrupt and uses it to tell
   when the button is pressed.

   The interrupt handler communicates the exact button press time to
   the task via a queue.

   This is a better example of how to wait for button input!
*/
void buttonIntTask(void *pvParameters) {
    printf("Waiting for button press interrupt on gpio %d...\r\n", gpio);
    QueueHandle_t *mainqueue = (QueueHandle_t *)pvParameters;
    gpio_set_interrupt(gpio, int_type, gpio_intr_handler);

    uint32_t last = 0;
    while(1) {
        uint32_t button_ts;
        xQueueReceive(*mainqueue, &button_ts, portMAX_DELAY);
        button_ts *= portTICK_PERIOD_MS;
        if(last < button_ts-200) {
            printf("Button interrupt fired at %dms\r\n", button_ts);
            flag = 1; //aciona bandeira
            last = button_ts;
        }
    }
}
 
static QueueHandle_t mainqueue;

void gpio_intr_handler(uint8_t gpio_num){
    uint32_t now = xTaskGetTickCountFromISR();
    xQueueSendToBackFromISR(mainqueue, &now, NULL);
}

void user_init(void) {
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());
    mainqueue = xQueueCreate(10, sizeof(uint32_t));
    
    gpio_enable(1, GPIO_OUTPUT);
    gpio_write(1, 1);
    gpio_enable(2, GPIO_OUTPUT);
    gpio_write(2, 1);
    gpio_enable(3, GPIO_OUTPUT);
    gpio_write(3, 1);
    gpio_enable(12, GPIO_OUTPUT);
    gpio_write(12, 1);
    gpio_enable(13, GPIO_OUTPUT);
    gpio_write(13, 1);
    gpio_enable(14, GPIO_OUTPUT);
    gpio_write(14, 1);
    gpio_enable(15, GPIO_OUTPUT);
    gpio_write(15, 1);  

    gpio_enable(verde, GPIO_OUTPUT);
    gpio_write(verde, 1);  
    gpio_enable(amarelo, GPIO_OUTPUT);
    gpio_write(amarelo, 0);  
    gpio_enable(vermelho, GPIO_OUTPUT);
    gpio_write(vermelho, 1);  
    xTaskCreate(farol, "farol", 256, &mainqueue, 2, NULL);

    //xTaskCreate(cronometro, "cronometro", 256, &mainqueue, 2, NULL);

    gpio_enable(gpio, GPIO_INPUT);
    xTaskCreate(buttonIntTask, "buttonIntTask", 256, &mainqueue, 2, NULL); 
}
