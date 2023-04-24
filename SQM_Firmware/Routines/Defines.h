#ifndef DEFINES_H_
#define DEFINES_H_

// RTC COMP value
// 1920 -> 60s
// 960 -> 30s
// 640 -> 20s
// 320 -> 10s
// 160 -> 5s
// 64 -> 2s
// 32 -> 1s

//#define CONF_RTC_COMP_VAL 1920	

#define AC_MEASUREMENT_INTERVAL_100ms	1

#define MAX_RETRIES_ON_SEND		3

#define DEF_ALFA	20
#define DEF_TMIN	1
#define DEF_PMIN_THRESHOLD	25
#define DEF_PMIN_LIMIT		10
#define DEF_PMAX_THRESHOLD  100
#define DEF_PMAX_LIMIT		200
#define DEF_DELTA			3

//#define TCP
//#define UDP
#define MQTT

//#define NFC

//#define USB_DEBUG

#endif /* DEFINES_H_ */