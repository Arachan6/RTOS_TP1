#ifndef TASK_GREEN_LED_H_
#define TASK_GREEN_LED_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

typedef struct {
    QueueHandle_t hgqueue;
} ao_green_led_handle_t;

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

void ao_green_led_init(ao_green_led_handle_t* hgao);

bool ao_green_led_send(ao_green_led_handle_t* hgao);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_LED_H_ */
/********************** end of file ******************************************/

