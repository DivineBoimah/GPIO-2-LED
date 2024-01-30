#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define GPIO_OUT_W1TS_REG 0x3FF44008
#define GPIO_OUT_W1TC_REG 0x3FF4400C
#define GPIO_ENABLE_REG   0x3FF44020

#define GPIO5             5
#define GPIO4             4

#define DELAY_MS_LED1     500
#define DELAY_MS_LED2     1000

typedef struct {
    volatile uint32_t * setReg;
    volatile uint32_t * clrReg;
} TaskParam;

void task_led1(void *);
void task_led2(void *);

void app_main(void)
{
    TaskParam * params = (TaskParam *)pvPortMalloc(sizeof(TaskParam));
    // Define pointers to GPIO registers
    //volatile uint32_t *gpio_out_w1ts_reg = (volatile uint32_t *)GPIO_OUT_W1TS_REG;
    //volatile uint32_t *gpio_out_w1tc_reg = (volatile uint32_t *)GPIO_OUT_W1TC_REG;
    volatile uint32_t *gpio_enable_reg = (volatile uint32_t *)GPIO_ENABLE_REG;
    
    *gpio_enable_reg = (1 << GPIO5) | (1 << GPIO4);
    
    if (params != NULL) {
        params->setReg = (volatile uint32_t *)GPIO_OUT_W1TS_REG;
        params->clrReg = (volatile uint32_t *)GPIO_OUT_W1TC_REG;
        
        // Create tasks for each LED
    	xTaskCreate(task_led1, "TaskLED1", 1000, (void *) params, 1, NULL);
    	xTaskCreate(task_led2, "TaskLED2", 1000, (void *) params, 1, NULL);
    }
}


void task_led1(void *pvParameters)
{
    TaskParam * par = (TaskParam *) pvParameters;
    
    volatile uint32_t *gpio_out_w1ts_reg_led1 = par->setReg;
    volatile uint32_t *gpio_out_w1tc_reg_led1 = par->clrReg;
    
    while (1)
    {
        // Toggle LED connected to GPIO5
        *gpio_out_w1ts_reg_led1 = (1 << GPIO5);
        vTaskDelay(DELAY_MS_LED1 /portTICK_PERIOD_MS);
        *gpio_out_w1tc_reg_led1 = (1 << GPIO5);
        vTaskDelay(DELAY_MS_LED1 /portTICK_PERIOD_MS);
    }
    
   
}


void task_led2(void *pvParameters)
{
    TaskParam * par = (TaskParam *) pvParameters;
    
    volatile uint32_t *gpio_out_w1ts_reg_led2 = par->setReg;
    volatile uint32_t *gpio_out_w1tc_reg_led2 = par->clrReg;
    
    while (1)
    {
        // Toggle LED connected to GPIO4
        *gpio_out_w1ts_reg_led2 = (1 << GPIO4);
        vTaskDelay(DELAY_MS_LED2 /portTICK_PERIOD_MS);
        *gpio_out_w1tc_reg_led2 = (1 << GPIO4);
        vTaskDelay(DELAY_MS_LED2 /portTICK_PERIOD_MS);
    }
    
   
}




