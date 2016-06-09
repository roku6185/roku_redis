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
  redis_serialization cmd_serialized = redis_serialize_object(command);
  return_code status = redis_write(cmd_serialized.value, cmd_serialized.length);
  free((char *)cmd_serialized.value);
  return status;
}