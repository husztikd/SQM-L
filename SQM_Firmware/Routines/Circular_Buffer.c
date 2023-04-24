#include "Circular_Buffer.h"


//----------------------------------------------------------------------------
void Circular_Buffer_init(CIRCULAR_BUFFER* cb, uint8_t* buffer, int64_t size)
{
    cb->buffer = buffer;
    cb->buffer_end = cb->buffer + size;
    cb->size = size;
    cb->data_start = cb->buffer;
    cb->data_end = cb->buffer;
    cb->count = 0;
    cb->free = cb->size - cb->count;    
}
//----------------------------------------------------------------------------
bool Circular_Buffer_push(CIRCULAR_BUFFER* cb, uint8_t data)
{
  uint8_t temp;

  if (cb == NULL || cb->buffer == NULL)
    return false;

#ifdef CIRCULAR_ROLLING
  if (Circular_Buffer_full(cb))
	Circular_Buffer_pop(cb, &temp)
#else
if (Circular_Buffer_full(cb))
    return false;
#endif

  *cb->data_end = data;
  cb->data_end++;
  if (cb->data_end == cb->buffer_end)
      cb->data_end = cb->buffer;
  cb->count++;
  cb->free = cb->size - cb->count;
  return true;
}
//----------------------------------------------------------------------------
bool Circular_Buffer_pop(CIRCULAR_BUFFER* cb, uint8_t* data)
{
  if (cb == NULL || cb->buffer == NULL)
      return false;

  *data = *cb->data_start;
  cb->data_start++;
  if (cb->data_start == cb->buffer_end)
      cb->data_start = cb->buffer;
  cb->count--;
      cb->free = cb->size - cb->count;
  return true;
}
//----------------------------------------------------------------------------
bool Circular_Buffer_clear(CIRCULAR_BUFFER* cb)
{
  if (cb == NULL || cb->buffer == NULL)
      return false;

  cb->data_start = cb->buffer;
  cb->data_end = cb->buffer;
  cb->count = 0;
  cb->free = cb->size - cb->count;    
  return true;
}
//----------------------------------------------------------------------------
bool Circular_Buffer_full(CIRCULAR_BUFFER* cb)
{
  return cb->count == cb->size;
}
//----------------------------------------------------------------------------
int Circular_Buffer_Stored_Data(CIRCULAR_BUFFER* cb) 
{
  return cb->count;
}
//----------------------------------------------------------------------------
int Circular_Buffer_Free_Space(CIRCULAR_BUFFER* cb) 
{
  return cb->free;
}
//----------------------------------------------------------------------------
