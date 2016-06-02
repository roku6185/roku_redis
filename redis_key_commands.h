#ifndef _REDIS_COMMANDS_H_
#define _REDIS_COMMANDS_H_

#include "redis_object.h"
#include "redis_return_codes.h"

return_code redis_del(int num_keys, ...);
return_code redis_exists(int num_keys, ...);
return_code redis_keys(const char *pattern);
return_code redis_randomkey();
return_code redis_ttl(const char *key);
return_code redis_expire(const char *key, int ttl_seconds);
return_code redis_ping(const char *message);

#endif