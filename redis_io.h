#ifndef _REDIS_WRITE_H_
#define _REDIS_WRITE_H_

#include "redis_return_codes.h"
#include "redis_object.h"

redis_object *redis_send_and_receive_command(redis_object *command, return_code *status);
return_code redis_send_command(redis_object *command);
return_code redis_read(char **result);
return_code redis_write(const char *data, int length);

#endif
