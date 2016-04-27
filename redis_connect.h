#ifndef _REDIS_CONNECT_H_
#define _REDIS_CONNECT_H_

#include <poll.h>
#include "redis_return_codes.h"

struct pollfd conn;

return_code redis_connect(const char *hostname, const char *port);
return_code redis_close();

#endif
