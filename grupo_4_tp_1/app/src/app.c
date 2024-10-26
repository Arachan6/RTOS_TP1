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

#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"

#include "task_button.h"
#include "task_led.h"
#include "task_ui.h"

/********************** macros and definitions *******************************/


/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration *****************************/

SemaphoreHandle_t hsem_button_pulse;
SemaphoreHandle_t hsem_button_short;
SemaphoreHandle_t hsem_button_long;

SemaphoreHandle_t hsem_led_red;
SemaphoreHandle_t hsem_led_green;
SemaphoreHandle_t hsem_led_blue;

/********************** external functions definition ************************/
void app_init(void)
{
	ao_button_init();

	hsem_led_red = xSemaphoreCreateBinary();
	hsem_led_green = xSemaphoreCreateBinary();
	hsem_led_blue = xSemaphoreCreateBinary();

	configASSERT(NULL != hsem_led_red);
	configASSERT(NULL != hsem_led_green);
	configASSERT(NULL != hsem_led_blue);

	vQueueAddToRegistry(hsem_led_red, "Red Semaphore");
	vQueueAddToRegistry(hsem_led_green, "Green Semaphore");
	vQueueAddToRegistry(hsem_led_blue, "Blue Semaphore");

	BaseType_t status;

	status = xTaskCreate(task_ui, "task_ui", 128, NULL, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);

	status = xTaskCreate(task_led, "task_led", 128, NULL, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);

	LOGGER_INFO("app init");

	cycle_counter_init();
}

/********************** end of file ******************************************/
