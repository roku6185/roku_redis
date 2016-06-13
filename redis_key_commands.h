#ifndef _REDIS_KEY_COMMANDS_H_
#define _REDIS_KEY_COMMANDS_H_

#include "redis_object.h"
#include "redis_return_codes.h"

return_code redis_del(int *response, int num_keys, ...);
return_code redis_exists(int *response, int num_keys, ...);
return_code redis_keys(redis_object **response, const char *pattern);
return_code redis_randomkey(redis_object **response);
return_code redis_ttl(int *response, const char *key);
return_code redis_expire(int *response, const char *key, int ttl_seconds);
return_code redis_ping(const char **response, const char *message);

#endif