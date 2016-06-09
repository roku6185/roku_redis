#ifndef _REDIS_HASH_COMMANDS_H_
#define _REDIS_HASH_COMMANDS_H_

#include "redis_object.h"
#include "redis_return_codes.h"

return_code redis_hdel(const char *key, int num_fields, ...);
return_code redis_hexists(const char *key, const char *field);
return_code redis_hget(const char *key, const char *field);
return_code redis_hgetall(const char *key);
return_code redis_hkeys(const char *key);
return_code redis_hlen(const char *key);
return_code redis_hset(const char *key, const char *field, const char *value);
return_code redis_hstrlen(const char *key, const char *field);
return_code redis_hvals(const char *key);

#endif