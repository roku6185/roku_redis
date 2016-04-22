#include <unistd.h>
#include "roku_redis_connect.h"
#include "roku_redis_return_codes.h"


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
