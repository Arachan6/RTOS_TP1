#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"

#include "task_red_led.h"

#define RED_LED_PERIOD_        			(1000)
#define RED_LED_QUEUE_LENGTH_     		(5)
#define RED_LED_QUEUE_ITEM_SIZE_  		(sizeof(bool))

bool ao_red_led_send (ao_red_led_handle_t *hrao){
	return (pdPASS == xQueueSend(hrao->hrqueue, true, 0));
}

void task_red_led(void *argument){
	TickType_t previousTime;
	ao_red_led_handle_t *hrao = (ao_red_led_handle_t *)argument;
	bool blink;
	while (true){
		if (pdPASS == xQueueReceive(hrao->hrqueue, &blink, portMAX_DELAY)){
			LOGGER_INFO("Blinking red led");
			previousTime = xTaskGetTickCount();
			HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
			vTaskDelayUntil(&previousTime, pdMS_TO_TICKS(RED_LED_PERIOD_));
			HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
		}
	}
}

void ao_red_led_init (ao_red_led_handle_t *hrao){
	hrao->hrqueue = xQueueCreate(RED_LED_QUEUE_LENGTH_, RED_LED_QUEUE_ITEM_SIZE_);
	while (NULL == hrao->hrqueue){}

	BaseType_t status;
	status = xTaskCreate(task_red_led, "task_ao_red_led", 128, (void*)hrao, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);
}
