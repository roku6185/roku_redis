#include <unistd.h>
#include <string.h>
#include "roku_redis_connect.h"
#include "roku_redis_return_codes.h"
#include "roku_redis_object.h"

return_code roku_redis_write(const char *data, int length)
{
  int bytes = 0;

  if(conn.fd)
  {
    bytes = write(conn.fd, data, length);
    return SUCCESS;
  }
  
  return SOCKET_WRITE_FAILED;
}

return_code roku_redis_send_command(redis_object *command)
{
  const char *tmp = redis_serialize_object(command);
  return roku_redis_write(tmp, strlen(tmp));
}