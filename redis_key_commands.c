#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "redis_io.h"
#include "redis_return_codes.h"

return_code redis_del(int *response, int num_keys, ...)
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
  return redis_send_then_wait_for_int(cmd, response);
}

return_code redis_exists(int *response, int num_keys, ...)
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
  return redis_send_then_wait_for_int(cmd, response);
}

return_code redis_expire(int *response, const char *key, int ttl_seconds)
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
  return redis_send_then_wait_for_int(cmd, response);
}

return_code redis_keys(redis_object **response, const char *pattern)
{
  redis_object *param1 = redis_create_bulk_string("KEYS");
  redis_object *param2 = redis_create_bulk_string(pattern);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_then_wait_for_object(cmd, response);
}

return_code redis_randomkey(redis_object **response)
{
  redis_object *param1 = redis_create_bulk_string("RANDOMKEY");
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  return redis_send_then_wait_for_object(cmd, response);
}

return_code redis_ttl(int *response, const char *key)
{
  redis_object *param1 = redis_create_bulk_string("TTL");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_then_wait_for_int(cmd, response);
}