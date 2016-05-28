#include <stdarg.h>
#include <string.h>
#include "redis_commands.h"
#include "redis_write.h"
#include "redis_return_codes.h"

return_code redis_del(int num_keys, ...)
{
  va_list args;
  va_start(args, num_keys);
  
  redis_object *cmd = redis_create_array();
  redis_object *param1 = redis_create_bulk_string("DEL");
  redis_array_push_back(cmd, param1);
  
  for(int i = 0; i < num_keys; i++)
  {
    const char *arg = va_arg(args, const char *);
    redis_object *param = redis_create_bulk_string(arg);
    redis_array_push_back(cmd, param);
  }
  
  va_end(args);
  return redis_send_command(cmd);
}