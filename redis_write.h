#ifndef _REDIS_WRITE_H_
#define _REDIS_WRITE_H_

#include "redis_return_codes.h"
#include "redis_object.h"

return_code redis_send_command(redis_object *command);
return_code redis_write(const char *data, int length);

#endif
