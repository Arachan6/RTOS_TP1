#ifndef TASK_BLUE_LED_H_
#define TASK_BLUE_LED_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

typedef struct {
    QueueHandle_t hbqueue;
} ao_blue_led_handle_t;

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

void ao_blue_led_init(ao_blue_led_handle_t* hbao);

bool ao_blue_led_send(ao_blue_led_handle_t* hbao);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_LED_H_ */
/********************** end of file ******************************************/

