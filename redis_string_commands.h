#ifndef _REDIS_STRING_COMMANDS_H_
#define _REDIS_STRING_COMMANDS_H_

#include "redis_object.h"
#include "redis_return_codes.h"

return_code redis_append(const char *key, const char *value, int *response);
return_code redis_get(const char *key, redis_object **response);
return_code redis_set(const char *key, const char *value, bool *response);
return_code redis_strlen(const char *key, int *response);

#endif