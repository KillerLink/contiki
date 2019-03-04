#include "contiki.h"
#include "contiki-lib.h"
#include <string.h>
#include "../common/common.h"
#include <stdio.h>

#define DEBUG DEBUG_PRINT

PROCESS(serial_hello_world_process, "serial hello world process");
AUTOSTART_PROCESSES(&serial_hello_world_process);


void init(void) {
	EVAL_LED1_OUTPUT;
	EVAL_LED2_OUTPUT;
	EVAL_LED4_OUTPUT;
	EVAL_LED1_OFF;
	EVAL_LED2_OFF;
	EVAL_LED4_OFF;
}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(serial_hello_world_process, ev, data) {
	static struct etimer et;

	PROCESS_BEGIN();
	printf("serial_hello_world process started\n");

	init();

	etimer_set(&et, 1*CLOCK_SECOND);

	while(1) {
		PROCESS_WAIT_EVENT();
		switch (ev) {
			case PROCESS_EVENT_TIMER: {
				if (etimer_expired(&et)) {
					etimer_set(&et, 1*CLOCK_SECOND);
					printf("Hello World!\n");
				}
				break;
			}
			case PROCESS_EVENT_MSG: {
				break;
			}
			default: {
				printf("unhandled event (%d)\n", ev);
			}
		}
	}
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
