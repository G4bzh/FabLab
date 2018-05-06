#include "osapi.h"
#include "user_interface.h"
#include "espconn.h"
#include <mem.h>

/*********************************************
 *
 * Constants
 *
 *********************************************/

#define SERVER_NAME  "192.168.188.153"
#define SERVER_IS_IP 1
#define SERVER_PORT  5000

/*********************************************
 *
 * Queue
 *
 *********************************************/

struct _send_queue
{
  uint8* data;
  uint16 len;
  struct _send_queue *prev;
  struct _send_queue *next;
};
typedef struct _send_queue send_queue_t;


/*********************************************
 *
 * Global variables
 *
 *********************************************/

struct espconn  myconn;
ip_addr_t myip;
esp_tcp mytcp;
os_timer_t timer_sent;
send_queue_t *send_head = NULL;
char buffer[256];

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABBBCDDD
 *                A : rf cal
 *                B : at parameters
 *                C : rf init data
 *                D : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }
    return rf_cal_sec;
}


/*********************************************
 *
 * Sent Callbacks
 *
 *********************************************/

void server_sent_cb( void *arg )
{
    struct espconn *conn = (struct espconn *)arg;

    /* Remove sent item from linked list */
    if (send_head->next != send_head)
    {
      send_queue_t *q;

      q = send_head;
      send_head->prev->next = send_head->next;
      send_head->next->prev = send_head->prev;
      send_head = send_head->next;

      os_free(q);
    }
    else
    {
      os_free(send_head);
      send_head = NULL;
    }


    /* We can now send next data - rearm timer */
    os_timer_arm(&timer_sent, 100, 1);

}


/*********************************************
 *
 * Send Timer Callback
 *
 *********************************************/


void server_send_cb(void *parg)
{
  struct espconn *conn = (struct espconn *)parg;

  if (send_head != NULL)
  {
    /* Callback will rearm the timer */
    os_timer_disarm(&timer_sent);

    espconn_regist_sentcb(conn, server_sent_cb);
    espconn_send(conn, send_head->data, send_head->len );
  }

}


/*********************************************
 *
 * Send enqueue
 *
 *********************************************/

 void send_enqueue(uint8* data, uint16 len)
 {
    send_queue_t *q;

    /* Create a new queue item */
    q = (send_queue_t*)os_malloc(sizeof(send_queue_t));
    if (q == NULL)
    {
      os_printf("Cannot allocate memory ofr sending");
      return;
    }

    q->data = data;
    q->len = len;

    /* Add it to the double linked list */
    if (send_head == NULL)
    {
      send_head = q;
      q->next = q;
      q->prev = q;
    }
    else
    {
      q->prev = send_head->prev;
      q->next = send_head;
      send_head->prev->next = q;
      send_head->prev = q;

    }
 }


/*********************************************
 *
 * TCP Connection Callbacks
 *
 *********************************************/

void server_connected_cb( void *arg )
{
    struct espconn *conn = (struct espconn *)arg;

    /* Set up a timer for sending data */
    os_timer_disarm(&timer_sent);
    os_timer_setfn(&timer_sent, (os_timer_func_t *)server_send_cb, conn);
    os_timer_arm(&timer_sent, 100, 1);


    os_sprintf( buffer, "POST %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n", "/esp/toto.txt", SERVER_NAME, 30);
    /* Enqueue data to send */
    send_enqueue(buffer,strlen(buffer));
    send_enqueue("1234567890",10);
    send_enqueue("abcdefghij",10);
    send_enqueue("\n1\n2\n3\n4\n5",10);
}


void server_disconnected_cb( void *arg )
{
    struct espconn *conn = (struct espconn *)arg;
    os_printf( "Disonnected !\n");
    //wifi_station_disconnect();
}


/*********************************************
 *
 * TCP Connection
 *
 *********************************************/


void ICACHE_FLASH_ATTR  server_connect(struct espconn *conn, ip_addr_t* ipaddr, int port)
{
  os_printf("Connecting to " IPSTR ":%d\n", *((uint8 *)&ipaddr->addr), *((uint8 *)&ipaddr->addr + 1), *((uint8 *)&ipaddr->addr + 2), *((uint8 *)&ipaddr->addr + 3), port);

  conn->type = ESPCONN_TCP;
  conn->state = ESPCONN_NONE;
  conn->proto.tcp=&mytcp;
  conn->proto.tcp->local_port = espconn_port();
  conn->proto.tcp->remote_port = port;
  os_memcpy( conn->proto.tcp->remote_ip, &ipaddr->addr, 4 );

  espconn_regist_connectcb( conn, server_connected_cb );
  espconn_regist_disconcb( conn, server_disconnected_cb );

  espconn_connect( conn );
 }


/*********************************************
 *
 * DNS found Callback
 *
 *********************************************/

LOCAL void ICACHE_FLASH_ATTR dns_found_cb(const char *name, ip_addr_t *ipaddr, void  *arg)
{
    struct espconn *conn = (struct espconn *)arg;

    if (ipaddr != NULL)
    {
      // os_printf("Server %s IP : " IPSTR "\n", name, *((uint8 *)&ipaddr->addr), *((uint8 *)&ipaddr->addr + 1), *((uint8 *)&ipaddr->addr + 2), *((uint8 *)&ipaddr->addr + 3));
      server_connect(conn, ipaddr, SERVER_PORT);
    }
    else
    {
      os_printf("Failed to resolve %s\n", name);
    }

}




/*********************************************
 *
 * Wifi Event Handler
 *
 *********************************************/

void wifi_handle_event_cb(System_Event_t *evt)
{
  os_printf("event %x\n", evt->event);

  switch (evt->event) {

    case EVENT_STAMODE_CONNECTED:
    {
      os_printf("connect to ssid %s, channel %d\n", evt->event_info.connected.ssid, evt->event_info.connected.channel);
      break;
    }

    case EVENT_STAMODE_DISCONNECTED:
    {
      os_printf("disconnect from ssid %s, reason %d\n", evt->event_info.disconnected.ssid, evt->event_info.disconnected.reason);
      break;
    }

    case EVENT_STAMODE_AUTHMODE_CHANGE:
    {
      // os_printf("mode: %d -> %d\n", evt->event_info.auth_change.old_mode, evt->event_info.auth_change.new_mode);
      break;
    }

    case EVENT_STAMODE_GOT_IP:
    {
      os_printf("ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR, IP2STR(&evt->event_info.got_ip.ip), IP2STR(&evt->event_info.got_ip.mask), IP2STR(&evt->event_info.got_ip.gw));
      os_printf("\n");


      if (SERVER_IS_IP)
      {
        /* Get IP */
        myip.addr = ipaddr_addr(SERVER_NAME);

        // os_printf("Server %s IP : "IPSTR "\n", SERVER_NAME, *((uint8 *)&myip.addr), *((uint8 *)&myip.addr + 1), *((uint8 *)&myip.addr + 2), *((uint8 *)&myip.addr + 3));
        server_connect(&myconn, &myip, SERVER_PORT);
      }
      else
      {
        /* Resolve Server Name */
        espconn_gethostbyname( &myconn, SERVER_NAME, &myip, dns_found_cb);
      }

      break;
    }

    default:
      break;
  }

  return;

}


/*********************************************
 *
 * User Init
 *
 *********************************************/

void ICACHE_FLASH_ATTR user_init(void)
{

  char ssid[32] = "Backoffice";
  char password[64] = "back4cyim";
  struct station_config stationConf;


    gpio_init();

    uart_init(115200, 115200);
    // os_printf("======\n");
    // os_printf("SDK version:%s\n", system_get_sdk_version());
    // os_printf("Chip ID:%d\n", system_get_chip_id());
    // os_printf("CPU freq : %d MHz\n", system_get_cpu_freq());
    // os_printf("Memory Map:\n");
    // system_print_meminfo();

    /* STA Mode */
    wifi_set_opmode(STATION_MODE);

    /* Mandatory */
    stationConf.bssid_set = 0;

    /* Fill config */
    os_memcpy(&stationConf.ssid, ssid, 32);
    os_memcpy(&stationConf.password, password, 64);

    /* Set config
     * Because we are in user_init, no need to call wifi_staion_connect()
     */
    wifi_station_set_config(&stationConf);

    wifi_set_event_handler_cb(wifi_handle_event_cb);
}
