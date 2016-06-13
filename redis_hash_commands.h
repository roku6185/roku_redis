#ifndef _REDIS_HASH_COMMANDS_H_
#define _REDIS_HASH_COMMANDS_H_

#include "redis_object.h"
#include "redis_return_codes.h"

return_code redis_hdel(int *response, const char *key, int num_fields, ...);
return_code redis_hexists(int *response, const char *key, const char *field);
return_code redis_hget(redis_object **response, const char *key, const char *field);
return_code redis_hgetall(redis_object **response, const char *key);
return_code redis_hkeys(redis_object **response, const char *key);
return_code redis_hlen(int *response, const char *key);
return_code redis_hset(int *response, const char *key, const char *field, const char *value);
return_code redis_hstrlen(int *response, const char *key, const char *field);
return_code redis_hvals(redis_object **response, const char *key);

#endif