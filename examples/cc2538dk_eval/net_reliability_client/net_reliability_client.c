#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "net/ip/resolv.h"
#include <string.h>
#include <stdbool.h>
#include "../common/common.h"

#include "net/ipv6/multicast/uip-mcast6-route.h"

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#define PROCESS_EVENT_USER1 (1)

static const char lorem_ipsum[] =
"Lorem ipsum dolor sit amet, consetetur sadipscing elitr,"
" sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.\n"
"At vero eos et accusam et justo duo dolores et ea rebum.\n"
"Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n"
"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor"
" invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.\n"
"At vero eos et accusam et justo duo dolores et ea rebum.\n"
"Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n"
"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor"
" invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.\n"
"At vero eos et accusam et justo duo dolores et ea rebum."
"Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n"
"\n"
"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat,"
" vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim"
" qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.\n"
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod"
" tincidunt ut laoreet dolore magna aliquam erat volutpat.\n"
"\n"
"Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis"
" nisl ut aliquip ex ea commodo consequat.\n"
"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat,"
" vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio"
" dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.\n"
"\n"
"Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming"
" id quod mazim placerat facer possim assum.\n"
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit,"
" sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat.\n"
;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&resolv_process,&udp_client_process);

static struct udp_socket socket;

static uip_ipaddr_t ipv6private;
static uip_ipaddr_t ipv6client;
static uip_ipaddr_t ipv6server;
static uip_ipaddr_t ipv6mclient;
static uip_ipaddr_t ipv6mserver;

static int bytesSent;
static int errorCount;

static void panic(void) {
	PRINTF("panic spin");
	EVAL_LED4_ON;
	while(1);
}


void init(void) {
	EVAL_LED1_OUTPUT;
	EVAL_LED2_OUTPUT;
	EVAL_LED4_OUTPUT;
	EVAL_LED1_OFF;
	EVAL_LED2_OFF;
	EVAL_LED4_OFF;
}


void recv_data (
	struct udp_socket *c,
	void *ptr,
	const uip_ipaddr_t *source_addr,
	uint16_t source_port,
	const uip_ipaddr_t *dest_addr,
	uint16_t dest_port,
	const uint8_t *data,
	uint16_t datalen)
{
	PRINTF("Received %d bytes\n", datalen);
}


void send_data(void) {
	int ret = 0;
	if (bytesSent==0) {
		PRINTF("starting cycle\n");
		EVAL_LED1_TOGGLE;
	}
	ret = udp_socket_send(&socket, lorem_ipsum, DATA_LENGTH);
	if (ret>=0) {
		EVAL_LED2_TOGGLE;
		bytesSent += ret;
	} else {
		errorCount++;
	}
	if (bytesSent >= DATA_TOTAL) {
		PRINTF("tx: %d, err: %d\n", bytesSent, errorCount);
		bytesSent = 0;
	} else {
	    process_post(&udp_client_process, PROCESS_EVENT_USER1, NULL);
	}
}


static void print_local_addresses(void) {
	int i;
	uint8_t state;
	PRINTF("==== IPV6 ADDRESS LIST ");
	for (i=0; i<UIP_DS6_ADDR_NB; i++) {
		state = uip_ds6_if.addr_list[i].state;
		if ( (uip_ds6_if.addr_list[i].isused)
			 && ((state == ADDR_TENTATIVE)
				 || (state == ADDR_PREFERRED))) {
			PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
			PRINTF("\n");
		}
	}
	PRINTF("==== MULTICAST ADDRESS LIST ====\n");
	uip_mcast6_route_t* mrt = uip_mcast6_route_list_head();
	for (i=0; i<uip_mcast6_route_count(); i++) {
		PRINT6ADDR(&(mrt->group));
		PRINTF("\n");
		mrt = mrt->next;
		if (mrt == NULL) break;
	}
}

void ip_setup (void) {
	uiplib_ipaddrconv(IPV6_PRIVATE_ADDR, &ipv6private);
	uiplib_ipaddrconv(IPV6_SERVER_ADDR, &ipv6server);
	uiplib_ipaddrconv(IPV6_CLIENT_ADDR, &ipv6client);
	uiplib_ipaddrconv(IPV6_SERVER_MCAST, &ipv6mserver);
	uiplib_ipaddrconv(IPV6_CLIENT_MCAST, &ipv6mclient);

	//add the network prefix
#if UIP_CONF_ROUTER
	uip_ds6_prefix_add(&ipv6private, 64, 0, 0, 0, 0);
#else
	uip_ds6_prefix_add(&ipv6private, 64, 0);
#endif
	//join appropriate multicast group
	if ( uip_ds6_maddr_add(&ipv6mclient) == NULL) {
		PRINTF("uip_ds6_maddr_add(mclient) failed!\n");
		panic();
	}
	//add a private network address
	if ( uip_ds6_addr_add(&ipv6client, 0, ADDR_MANUAL) == NULL) {
		PRINTF("uip_ds6_addr_add() failed!");
		panic();
	}
}

static void udp_start (void) {
	if (udp_socket_register(&socket, NULL, recv_data) != 1) {
		PRINTF("udp_socket_register failed!");
		panic();
	}
/*
	if (udp_socket_bind(&socket, UIP_HTONS(SERVER_PORT)) != 1) {
		PRINTF("udp_socket_bind failed!");
		panic();
	}
*/
	if (udp_socket_connect(&socket, &ipv6server, UIP_HTONS(SERVER_PORT)) != 1) {
		PRINTF("udp_socket_bind failed!");
		panic();
	}
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data) {
	static struct etimer et;

	PROCESS_BEGIN();
	PRINTF("net_throughput_client process started\n");

	init();
	ip_setup();

	PRINTF("Waiting for IPv6 Address configuration...\n");
	etimer_set(&et, 15*CLOCK_SECOND);
	while (!etimer_expired(&et)) {
	    PROCESS_YIELD();
	}
	PRINTF("Network should be ready now!\n");

	print_local_addresses();
	udp_start();

	PRINTF("UDP started\n");

    process_post(&udp_client_process, PROCESS_EVENT_USER1, NULL);
	while(1) {
		PROCESS_WAIT_EVENT();
		switch (ev) {
			case PROCESS_EVENT_TIMER: {
				if (etimer_expired(&et)) {
					etimer_set(&et, 1*CLOCK_SECOND);
				}
				break;
			}
			case PROCESS_EVENT_MSG: {
				break;
			}
			case PROCESS_EVENT_USER1: {
				send_data();
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
