#ifndef _ROKU_REDIS_WRITE_
#define _ROKU_REDIS_WRITE_

#include "roku_redis_return_codes.h"
#include "roku_redis_object.h"

return_code roku_redis_send_command(redis_object *command);
return_code roku_redis_write(const char *data, int length);

#endif
