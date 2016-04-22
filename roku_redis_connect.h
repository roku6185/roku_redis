#ifndef _ROKU_REDIS_CONNECT_
#define _ROKU_REDIS_CONNECT_

#include <poll.h>
#include "roku_redis_return_codes.h"

struct pollfd conn;

return_code roku_redis_connect(const char *hostname, int port);
return_code roku_redis_close();

#endif
