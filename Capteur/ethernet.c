#include <stdint.h>

#include "ethernet.h"
#include "w5100.h"

/** Function for Ethernet hardware **/

void ethernet_init(uint8_t *mac,uint8_t *ip,uint8_t *gateway,uint8_t *mask){
iinchip_init();
sysinit(0x55,0x55);
setSHAR(mac);
setSIPR(ip);
setGAR(gateway);
setSUBR(mask);
}
