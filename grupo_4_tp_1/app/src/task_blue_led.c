#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"

#include "task_blue_led.h"

#define BLUE_LED_PERIOD_       			(1000)
#define BLUE_LED_QUEUE_LENGTH_     		(5)
#define BLUE_LED_QUEUE_ITEM_SIZE_  		(sizeof(bool))

bool ao_blue_led_send (ao_blue_led_handle_t *hbao){
	return (pdPASS == xQueueSend(hbao->hbqueue, true, 0));
}

void task_blue_led(void *argument){
	TickType_t previousTime;
	ao_blue_led_handle_t *hbao = (ao_blue_led_handle_t *)argument;
	bool blink;
	while (true){
		if (pdPASS == xQueueReceive(hbao->hbqueue, &blink, portMAX_DELAY)){
			LOGGER_INFO("Blinking blue led");
			previousTime = xTaskGetTickCount();
			HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_SET);
			vTaskDelayUntil(&previousTime, pdMS_TO_TICKS(BLUE_LED_PERIOD_));
			HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);
		}
	}
}

void ao_blue_led_init (ao_blue_led_handle_t *hbao){
	hbao->hbqueue = xQueueCreate(BLUE_LED_QUEUE_LENGTH_, BLUE_LED_QUEUE_ITEM_SIZE_);
	while (NULL == hbao->hbqueue){}

	BaseType_t status;
	status = xTaskCreate(task_blue_led, "task_ao_blue_led", 128, (void*)hbao, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);
}
