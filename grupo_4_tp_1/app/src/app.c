#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"

#include "task_button.h"
#include "task_red_led.h"
#include "task_green_led.h"
#include "task_blue_led.h"
#include "task_ui.h"

ao_ui_handle_t ao_ui;
ao_red_led_handle_t ao_red_led_h;
ao_green_led_handle_t ao_green_led_h;
ao_blue_led_handle_t ao_blue_led_h;

void app_init(void){

	ao_ui_init(&ao_ui);
	ao_red_led_init (&ao_red_led_h);
	ao_green_led_init (&ao_green_led_h);
	ao_blue_led_init (&ao_blue_led_h);

	BaseType_t status;
	status = xTaskCreate(task_button, "task_ui", 128, NULL, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);

	LOGGER_INFO("Application successfully initialized");

	cycle_counter_init();
}
