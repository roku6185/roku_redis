#include <stdio.h>
#include <stdarg.h>
#include <string.h>
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

return_code redis_exists(int num_keys, ...)
{
  va_list args;
  va_start(args, num_keys);
  
  redis_object *cmd = redis_create_array();
  redis_object *param1 = redis_create_bulk_string("EXISTS");
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

return_code redis_expire(const char *key, int ttl_seconds)
{
  redis_object *param1 = redis_create_bulk_string("EXPIRE");
  redis_object *param2 = redis_create_bulk_string(key);
  
  // TODO: refactor this int -> string conversion
  char ttl_str[512];
  snprintf(ttl_str, 512, "%d", ttl_seconds);
  redis_object *param3 = redis_create_bulk_string(ttl_str);
  
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  redis_array_push_back(cmd, param3);
  return redis_send_command(cmd);
}

return_code redis_keys(const char *pattern)
{
  redis_object *param1 = redis_create_bulk_string("KEYS");
  redis_object *param2 = redis_create_bulk_string(pattern);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_command(cmd);
}

return_code redis_randomkey()
{
  redis_object *param1 = redis_create_bulk_string("RANDOMKEY");
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  return redis_send_command(cmd);
}

return_code redis_ttl(const char *key)
{
  redis_object *param1 = redis_create_bulk_string("TTL");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_command(cmd);
}