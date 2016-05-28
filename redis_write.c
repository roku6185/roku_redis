#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "redis_connect.h"
#include "redis_return_codes.h"
#include "redis_object.h"

return_code redis_write(const char *data, int length)
{
  int bytes = 0;

  if(conn.fd)
  {
    bytes = write(conn.fd, data, length);
    return SUCCESS;
  }
  
  return SOCKET_WRITE_FAILED;
}

return_code redis_send_command(redis_object *command)
{
  redis_str_int tuple = redis_serialize_object(command);
  return_code status = redis_write(tuple.value, tuple.length);
  free((char *)tuple.value);
  return status;
}