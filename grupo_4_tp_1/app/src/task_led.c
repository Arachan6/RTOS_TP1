/*
 * Copyright (c) 2023 Sebastian Bedin <sebabedin@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @author : Sebastian Bedin <sebabedin@gmail.com>
 */

/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"

/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_           (1000)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

typedef enum
{
  LED_COLOR_NONE,
  LED_COLOR_RED,
  LED_COLOR_GREEN,
  LED_COLOR_BLUE,
  LED_COLOR_WHITE,
  LED_COLOR__N,
} led_color_t;

/********************** external data definition *****************************/

extern SemaphoreHandle_t hsem_led_red;
extern SemaphoreHandle_t hsem_led_green;
extern SemaphoreHandle_t hsem_led_blue;

/********************** internal functions definition ************************/

void led_set_colors(bool r, bool g, bool b)
{
  HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, r ? GPIO_PIN_SET: GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, g ? GPIO_PIN_SET: GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, b ? GPIO_PIN_SET: GPIO_PIN_RESET);
}

/********************** external functions definition ************************/
bool ao_led_red_receive ()
{
	return (pdTRUE == xSemaphoreTake(hsem_led_red, 0));
}

bool ao_led_green_receive ()
{
	return (pdTRUE == xSemaphoreTake(hsem_led_green, 0));
}

bool ao_led_blue_receive ()
{
	return (pdTRUE == xSemaphoreTake(hsem_led_blue, 0));
}

void task_led_red(void *argument)
{
  while (true)
  {
	  if (ao_led_red_receive())
	  {
		LOGGER_INFO("led red");
		led_set_colors(true, false, false);
	  } else
	  {
		led_set_colors(false, false, false);
	  }

	vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));
  }
}

void task_led_green(void *argument)
{
  while (true)
  {
	  if (ao_led_green_receive())
	  {
		LOGGER_INFO("led green");
		led_set_colors(false, true, false);
	  } else
	  {
		led_set_colors(false, false, false);
	  }

	vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));
  }
}

void task_led_blue(void *argument)
{
  while (true)
  {
	  if (ao_led_blue_receive())
	  {
		LOGGER_INFO("led blue");
		led_set_colors(false, false, true);
	  } else
	  {
		led_set_colors(false, false, false);
	  }

	vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));
  }
}

void ao_led_red_init ()
{
	hsem_led_red = xSemaphoreCreateBinary();

	configASSERT(NULL != hsem_led_red);

	vQueueAddToRegistry(hsem_led_red, "Red Semaphore");

	BaseType_t status;
	status = xTaskCreate(task_led_red, "task_led_red", 128, NULL, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);
}

void ao_led_green_init ()
{
	hsem_led_green = xSemaphoreCreateBinary();

	configASSERT(NULL != hsem_led_green);

	vQueueAddToRegistry(hsem_led_green, "Green Semaphore");

	BaseType_t status;
	status = xTaskCreate(task_led_green, "task_led_green", 128, NULL, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);
}

void ao_led_blue_init ()
{
	hsem_led_blue = xSemaphoreCreateBinary();

	configASSERT(NULL != hsem_led_blue);

	vQueueAddToRegistry(hsem_led_blue, "Blue Semaphore");

	BaseType_t status;
	status = xTaskCreate(task_led_blue, "task_led_blue", 128, NULL, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);
}

/********************** end of file ******************************************/
