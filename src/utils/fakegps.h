#ifndef _FAKEGPS_H
#define _FAKEGPS_H

#define   GEOIP_URL     "http://ip-api.com/json/"
#define   FAKEGPS_SYNC_REQUEST      _BV(1)

void fakegps_setup(void);
double fakegps_get_last_lat(void);
double fakegps_get_last_lon(void);

#endif
