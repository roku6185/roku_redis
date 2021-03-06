#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "redis_connect.h"
#include "redis_return_codes.h"
#include "redis_object.h"

return_code redis_write(const char *data, int length)
{
  if(conn.fd <= 0)
    return SOCKET_WRITE_FAILED;

  while(length > 0)
  {
    int bytes = write(conn.fd, data, length);
    
    if(bytes == 0)
    {
      return SOCKET_DISCONNECT;
    }
    else if(bytes == -1)
    {
      if(errno == EAGAIN)
      {
        // Blocked. Try again.
        continue;
      }
      else if(errno == EINTR)
      {
        // Interrupted by a signal. Try again.
        continue;
      }
      else
      {
        return SOCKET_WRITE_FAILED;
      }
    }
    
    length -= bytes;
    data += bytes;
  }
  
  return SUCCESS;
}

return_code redis_read(char **result)
{
  conn.events = POLLIN;
  conn.revents = 0;
  int poll_status = poll(&conn, 1, 10000);
  
  if(poll_status == 0)
  {
    return SOCKET_TIMEOUT;
  }

  int total_bytes = 0;
  int bytes;
  int heap_buffer_size = 1024;
  char *heap_buffer = malloc(heap_buffer_size);

  int tmp_buffer_size = 256;
  char tmp_buffer[tmp_buffer_size];

  while(conn.revents & POLLIN)
  {
    bytes = read(conn.fd, tmp_buffer, tmp_buffer_size);
    
    if(bytes == 0)
    {
      break;
    }
    else if(bytes == -1)
    {
      if(errno == EAGAIN)
      {
        // Blocked. Try again.
        continue;
      }
      else if(errno == EINTR)
      {
        // Interrupted by signal. Try again.
        continue;
      }
      else
      {
        return SOCKET_READ_FAILED;
      }
    }
      
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
    conn.events = POLLIN;
    conn.revents = 0;
  }
  
  heap_buffer[total_bytes] = 0;
  *result = heap_buffer;
  return SUCCESS;
}

return_code redis_send_command(redis_object *command)
{
  redis_serialization cmd_serialized = redis_serialize_object(command);
  // Don't send trailing null byte
  return_code status = redis_write(cmd_serialized.value, cmd_serialized.length - 1);
  free((char *)cmd_serialized.value);
  return status;
}


return_code redis_send_and_receive_command(redis_object *cmd, redis_object **response)
{
  return_code status;
  
  if((status = redis_send_command(cmd)) == SUCCESS)
  {
    char *buffer = NULL;
    
    if((status = redis_read(&buffer)) == SUCCESS)
    {
      *response = redis_deserialize_object(buffer);      
      
      if(buffer != NULL)
        free(buffer);
      
      return SUCCESS;
    }
  }
  
  return status;
}

return_code redis_send_then_wait_for_int(redis_object *cmd, int *response)
{
  return_code status;
  redis_object *obj = NULL;
  *response = -1;
  
  if((status = redis_send_and_receive_command(cmd, &obj)) == SUCCESS)
  {
    if((status = redis_object_to_integer(obj, response)) == SUCCESS)
      return SUCCESS;
  }
  
  return status;
}

return_code redis_send_then_wait_for_ok(redis_object *cmd, bool *response)
{
  return_code status;
  redis_object *obj = NULL;
  *response = FALSE;
  
  if((status = redis_send_and_receive_command(cmd, &obj)) == SUCCESS)
  {
    const char *reply = NULL;
    
    if((status = redis_object_to_string(obj, &reply)) == SUCCESS)
    {
      if(reply && (strncmp(reply, "OK", 2) == 0))
        *response = TRUE;
    }
  }
  
  return status;
}

return_code redis_send_then_wait_for_object(redis_object *cmd, redis_object **response)
{
  return_code status;
  
  if((status = redis_send_and_receive_command(cmd, response)) == SUCCESS)
    return SUCCESS;
    
  return status;
}