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
	&cc2538_gpio_basic_process
);

/*---------------------------------------------------------------------------*/

PROCESS_THREAD(cc2538_gpio_basic_process, ev, data)
{
	PROCESS_BEGIN();

	while (1) {
		leds_off(LEDS_RED);
		leds_on(LEDS_RED);
		leds_off(LEDS_RED);
		leds_on(LEDS_RED);
		leds_off(LEDS_RED);
		leds_on(LEDS_RED);
		leds_off(LEDS_RED);
		leds_on(LEDS_RED);
		leds_off(LEDS_RED);
		leds_on(LEDS_RED);
		leds_off(LEDS_RED);
		leds_on(LEDS_RED);
		leds_off(LEDS_RED);
		leds_on(LEDS_RED);
		leds_off(LEDS_RED);
		leds_on(LEDS_RED);
	}

	PROCESS_END();
}


