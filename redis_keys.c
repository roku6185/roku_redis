#include <string.h>
#include "redis_commands.h"
#include "redis_write.h"
#include "redis_return_codes.h"

return_code redis_keys(const char *pattern)
{
  redis_object *param1 = redis_create_bulk_string("KEYS", 4);
  redis_object *param2 = redis_create_bulk_string(pattern, strlen(pattern));
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_command(cmd);
}