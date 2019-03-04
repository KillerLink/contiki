#include "contiki.h"
#include "cpu.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include <stdio.h>
#include <stdint.h>


/*---------------------------------------------------------------------------*/

PROCESS(cc2538_gpio_basic_process, "proc_gpio_basic");
PROCESS(cc2538_interrupt_process, "proc_interrupt");
AUTOSTART_PROCESSES(
	&cc2538_gpio_basic_process,
	&cc2538_interrupt_process
);

/*---------------------------------------------------------------------------*/

PROCESS_THREAD(cc2538_gpio_basic_process, ev, data)
{
	PROCESS_BEGIN();

	SENSORS_ACTIVATE(button_select_sensor);
	while(1) {
		PROCESS_YIELD();
	}

	PROCESS_END();
}


PROCESS_THREAD(cc2538_interrupt_process, ev, data)
{
	PROCESS_BEGIN();
	while(1) {
		PROCESS_WAIT_EVENT_UNTIL((ev == sensors_event) && (data == &button_select_sensor));
		leds_toggle(LEDS_RED);
	}
	PROCESS_END();
}
