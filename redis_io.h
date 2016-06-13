#ifndef _REDIS_WRITE_H_
#define _REDIS_WRITE_H_

#include "redis_return_codes.h"
#include "redis_object.h"

return_code redis_send_then_wait_for_int(redis_object *cmd, int *response);
return_code redis_send_then_wait_for_ok(redis_object *cmd, bool *response);
return_code redis_send_then_wait_for_object(redis_object *cmd, redis_object **response);

return_code redis_send_and_receive_command(redis_object *cmd, redis_object **response);
return_code redis_send_command(redis_object *cmd);

return_code redis_read(char **result);
return_code redis_write(const char *data, int length);

#endif
