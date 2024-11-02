#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"

#include "task_green_led.h"

#define GREEN_LED_PERIOD_        			(1000)
#define GREEN_LED_QUEUE_LENGTH_     		(5)
#define GREEN_LED_QUEUE_ITEM_SIZE_  		(sizeof(bool))

bool ao_green_led_send (ao_green_led_handle_t *hgao){
	return (pdPASS == xQueueSend(hgao->hgqueue, true, 0));
}

void task_green_led(void *argument){
	TickType_t previousTime;
	ao_green_led_handle_t *hgao = (ao_green_led_handle_t *)argument;
	bool blink;
	while (true){
		if (pdPASS == xQueueReceive(hgao->hgqueue, &blink, portMAX_DELAY)){
			LOGGER_INFO("Blinking green led");
			previousTime = xTaskGetTickCount();
			HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
			vTaskDelayUntil(&previousTime, pdMS_TO_TICKS(GREEN_LED_PERIOD_));
			HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
		}
	}
}

void ao_green_led_init (ao_green_led_handle_t *hgao){
	hgao->hgqueue = xQueueCreate(GREEN_LED_QUEUE_LENGTH_, GREEN_LED_QUEUE_ITEM_SIZE_);
	while (NULL == hgao->hgqueue){}

	BaseType_t status;
	status = xTaskCreate(task_green_led, "task_ao_green_led", 128, (void*)hgao, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);
}
