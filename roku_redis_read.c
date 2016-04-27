#include "roku_redis_connect.h"
#include "roku_redis_return_codes.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

return_code roku_redis_read(char **result)
{
  int poll_status = poll(&conn, 1, 10000);

  if(poll_status == 0)
  {
    return SOCKET_TIMEOUT;
  }

  int total_bytes = 0;
  int bytes;
  int heap_buffer_size = 10;
  char *heap_buffer = malloc(heap_buffer_size);

  int tmp_buffer_size = 5;
  char tmp_buffer[tmp_buffer_size];

  while(conn.revents & POLLIN)
  {
    bytes = recv(conn.fd, tmp_buffer, tmp_buffer_size, MSG_DONTWAIT);
    
    if(bytes <= 0)
      break;
      
    tmp_buffer[bytes] = 0;
    total_bytes += bytes;

    if(total_bytes > heap_buffer_size)
    {
      heap_buffer_size = heap_buffer_size * 10;
      char *new_heap_ptr = realloc(heap_buffer, heap_buffer_size);

      if(new_heap_ptr)
        heap_buffer = new_heap_ptr;
      else
      {
        return SOCKET_MEMORY_FAILED;
      }
     }

    memcpy(heap_buffer + total_bytes - bytes, tmp_buffer, bytes);
  }

  heap_buffer[total_bytes] = 0;
  *result = heap_buffer;
  return SUCCESS;
}
