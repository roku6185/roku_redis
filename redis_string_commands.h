#ifndef _REDIS_STRING_COMMANDS_H_
#define _REDIS_STRING_COMMANDS_H_

#include "redis_object.h"
#include "redis_return_codes.h"

int redis_append(const char *key, const char *value, return_code *status);
return_code redis_get(const char *key);
return_code redis_set(const char *key, const char *value);
return_code redis_strlen(const char *key);

#endif