#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include "Common.h"

//----------------------------------------------------------------------------

typedef struct
{
  uint8_t* buffer;
  uint8_t* buffer_end;
  uint8_t* data_start;
  uint8_t* data_end;
  uint64_t count;
  uint64_t size;
  uint64_t free;
 } CIRCULAR_BUFFER;


//----------------------------------------------------------------------------
void Circular_Buffer_init(CIRCULAR_BUFFER* cb, uint8_t* buffer, int64_t size);
bool Circular_Buffer_push(CIRCULAR_BUFFER* cb, uint8_t data);
bool Circular_Buffer_pop(CIRCULAR_BUFFER* cb, uint8_t* data);
bool Circular_Buffer_clear(CIRCULAR_BUFFER* cb);
bool Circular_Buffer_full(CIRCULAR_BUFFER* cb);
int Circular_Buffer_Stored_Data(CIRCULAR_BUFFER* cb);
int Circular_Buffer_Free_Space(CIRCULAR_BUFFER* cb);
//----------------------------------------------------------------------------

#endif // _CIRCULAR_BUFFER_H_
