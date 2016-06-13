#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "redis_io.h"
#include "redis_return_codes.h"

return_code redis_hdel(int *response, const char *key, int num_fields, ...)
{
  va_list args;
  va_start(args, num_fields);
  
  redis_object *cmd = redis_create_array();
  redis_object *param1 = redis_create_bulk_string("HDEL");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  
  for(int i = 0; i < num_fields; i++)
  {
    const char *arg = va_arg(args, const char *);
    redis_object *param = redis_create_bulk_string(arg);
    redis_array_push_back(cmd, param);
  }
  
  va_end(args);
  return redis_send_then_wait_for_int(cmd, response);
}

return_code redis_hexists(int *response, const char *key, const char *field)
{
  redis_object *cmd = redis_create_array();
  redis_object *param1 = redis_create_bulk_string("HEXISTS");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *param3 = redis_create_bulk_string(field);
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  redis_array_push_back(cmd, param3);
  return redis_send_then_wait_for_int(cmd, response);
}

return_code redis_hget(redis_object **response, const char *key, const char *field)
{
  redis_object *cmd = redis_create_array();
  redis_object *param1 = redis_create_bulk_string("HGET");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *param3 = redis_create_bulk_string(field);
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  redis_array_push_back(cmd, param3);
  return redis_send_then_wait_for_object(cmd, response);
}

return_code redis_hgetall(redis_object **response, const char *key)
{
  redis_object *cmd = redis_create_array();
  redis_object *param1 = redis_create_bulk_string("HGETALL");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_then_wait_for_object(cmd, response);
}

return_code redis_hkeys(redis_object **response, const char *key)
{
  redis_object *cmd = redis_create_array();
  redis_object *param1 = redis_create_bulk_string("HKEYS");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_then_wait_for_object(cmd, response); 
}

return_code redis_hlen(int *response, const char *key)
{
  redis_object *cmd = redis_create_array();
  redis_object *param1 = redis_create_bulk_string("HLEN");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_then_wait_for_int(cmd, response);
}

return_code redis_hset(redis_object **response, const char *key, const char *field, const char *value)
{
  redis_object *cmd = redis_create_array();
  redis_object *param1 = redis_create_bulk_string("HSET");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *param3 = redis_create_bulk_string(field);
  redis_object *param4 = redis_create_bulk_string(value);
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  redis_array_push_back(cmd, param3);
  redis_array_push_back(cmd, param4);
  return redis_send_then_wait_for_object(cmd, response); 
}

return_code redis_hstrlen(int *response, const char *key, const char *field)
{
  redis_object *cmd = redis_create_array();
  redis_object *param1 = redis_create_bulk_string("HSTRLEN");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *param3 = redis_create_bulk_string(field);
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  redis_array_push_back(cmd, param3);
  return redis_send_then_wait_for_int(cmd, response);
}

return_code redis_hvals(redis_object **response, const char *key)
{
  redis_object *cmd = redis_create_array();
  redis_object *param1 = redis_create_bulk_string("HVALS");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_then_wait_for_object(cmd, response);
}