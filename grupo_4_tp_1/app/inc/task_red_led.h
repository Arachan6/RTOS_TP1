#ifndef TASK_RED_LED_H_
#define TASK_RED_LED_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

typedef struct {
    QueueHandle_t hrqueue;
} ao_red_led_handle_t;

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

void ao_red_led_init(ao_red_led_handle_t* hrao);

bool ao_red_led_send(ao_red_led_handle_t* hrao);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_LED_H_ */
/********************** end of file ******************************************/

