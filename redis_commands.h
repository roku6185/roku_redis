#ifndef _REDIS_COMMANDS_H_
#define _REDIS_COMMANDS_H_

#include "redis_object.h"
#include "redis_return_codes.h"

return_code redis_ping(const char *message);

#endif