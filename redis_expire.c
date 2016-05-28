#include <stdio.h>
#include <string.h>
#include "redis_commands.h"
#include "redis_write.h"
#include "redis_return_codes.h"

return_code redis_expire(const char *key, int ttl_seconds)
{
  redis_object *param1 = redis_create_bulk_string("EXPIRE", 6);
  redis_object *param2 = redis_create_bulk_string(key, strlen(key));
  
  // TODO: refactor this int -> string conversion
  char ttl_str[512];
  snprintf(ttl_str, 512, "%d", ttl_seconds);
  redis_object *param3 = redis_create_bulk_string(ttl_str, strlen(ttl_str));
  
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  redis_array_push_back(cmd, param3);
  return redis_send_command(cmd);
}