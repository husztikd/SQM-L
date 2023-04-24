#ifndef DEBUG_H_
#define DEBUG_H_

#include "Common.h"


int debug_write_bytes(uint8_t *data, uint32_t size);
int debug_write_string(char *str);
void Debug_Printf(const char *fmt, ...);



#endif /* DEBUG_H_ */