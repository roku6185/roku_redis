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
  const char *tmp = redis_serialize_object(command);
  return redis_write(tmp, strlen(tmp));
}