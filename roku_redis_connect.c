#include "roku_redis_connect.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>


return_code roku_redis_connect(const char *hostname, const char *port)
{
  int fd = -1, rv;
  struct addrinfo hints, *servinfo, *p;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0)
  {
    return SOCKET_ADDRESS_TRANSLATION_FAILED;
  }

  for(p = servinfo; p != NULL; p = p->ai_next)
  {
    if((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
    {
      continue;
    }

    if(connect(fd, p->ai_addr, p->ai_addrlen) == 0)
      break;

    close(fd);
    fd = -1;
  }

  if(fd < 0)
    return SOCKET_CONNECT_FAILED;

  conn.fd = fd;
  conn.events = POLLIN;

  freeaddrinfo(servinfo);
  return SUCCESS;
}

return_code roku_redis_close()
{
  if(conn.fd)
  {
    close(conn.fd);
    conn.fd = -1;
    return SUCCESS;
  }

  return SOCKET_CLOSE_FAILED;
}
