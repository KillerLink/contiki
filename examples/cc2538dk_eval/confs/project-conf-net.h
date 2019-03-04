#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

#define CONTIKI_WITH_IPV6 1
#define CONTIKI_WITH_RPL 0
#ifdef UIP_CONF_IPV6_RPL
	#undef UIP_CONF_IPV6_RPL
	#define UIP_CONF_IPV6_RPL 0 // also set by Makefile.inc
#endif
#define UIP_CONF_RPL 0
#define QUEUEBUF_CONF_NUM 32-8
#define QUEUEBUF_CONF_REF_NUM 32-8
#define SICSLOWPAN_CONF_FRAGMENT_BUFFERS 32-8
#define UIP_CONF_CHECKS 1
#define NETSTACK_CONF_RDC nullrdc_driver
//#define NETSTACK_CONF_RDC sicslowmac_driver
#define UIP_CONF_ND6_DEF_MAXDADNS 1
#define UIP_CONF_IPV6_QUEUE_PKT 1
#define SICSLOWPAN_CONF_FRAG 1
#define UIP_CONF_IPV6_REASSEMBLY 1
#define UIP_CONF_BUFFER_SIZE 1200
#define NETSTACK_CONF_WITH_IPV6 1
#define SICSLOWPAN_CONF_COMPRESSION HC1
#define UIP_CONF_ROUTER 0
#define RESOLV_CONF_SUPPORTS_MDNS 0
#define UIP_CONF_ND6_SEND_NS 1
#define UIP_CONF_ND6_SEND_NA 1
#define UIP_CONF_ND6_SEND_RA 0
#define UIP_CONF_ND6_SEND_RS 0
#define UIP_CONF_DS6_LL_NUD 1
#define RF_CHANNEL 26
#define CC2538_RF_CONF_TX_POWER 0x49 //0x49 == 0dbm
#define IEEE802154_CONF_PANID 0x1337
#define WATCHDOG_CONF_ENABLE 0
#define UIP_IPV6_MULTICAST 1
#define UIP_MCAST6_CONF_ENGINE UIP_MCAST6_ENGINE_ROLL_TM
//#define UIP_CONF_DS6_MADDR_NBU 8 //idk?
//#define UIP_MCAST6_ROUTE_CONF_ROUTES 4 //idk?
//#define UIP_CONF_MCAST6_ENGINE UIP_MCAST6_ENGINE_ESMRF
//
#define NBR_TABLE_CONF_MAX_NEIGHBORS 8
#define UIP_NETIF_MAX_ADDRESSES  8
#define UIP_ND6_MAX_PREFIXES     8
#define UIP_CONF_ICMP6            1
#define UIP_CONF_UDP              1
#define SICSLOWPAN_CONF_MAX_ADDR_CONTEXTS 4
#define SICSLOWPAN_CONF_FRAG 1
//
/*
#define SICSLOWPAN_CONF_ADDR_CONTEXT_2 {\
	addr_contexts[2].prefix[0]=0xfd;\
	addr_contexts[2].prefix[1]=0x0b;\
	addr_contexts[2].prefix[2]=0xad;\
	addr_contexts[2].prefix[3]=0x46;\
	addr_contexts[2].prefix[4]=0x74;\
	addr_contexts[2].prefix[5]=0xe5;\
	addr_contexts[2].prefix[6]=0xde;\
	addr_contexts[2].prefix[7]=0x4c;\
}
*/

#endif

