#ifndef _REDIS_CONNECTION_COMMANDS_H_
#define _REDIS_CONNECTION_COMMANDS_H_

#include "redis_object.h"
#include "redis_return_codes.h"

return_code redis_echo(const char *message);
return_code redis_ping(const char *message);
return_code redis_quit();

#endif