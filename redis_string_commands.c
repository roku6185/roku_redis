#include <string.h>
#include "redis_write.h"
#include "redis_return_codes.h"

return_code redis_append(const char *key, const char *value)
{
  redis_object *param1 = redis_create_bulk_string("APPEND");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *param3 = redis_create_bulk_string(value);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  redis_array_push_back(cmd, param3);
  return redis_send_command(cmd);
}

return_code redis_get(const char *key)
{
  redis_object *param1 = redis_create_bulk_string("GET");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_command(cmd);
}

return_code redis_set(const char *key, const char *value)
{
  redis_object *param1 = redis_create_bulk_string("SET");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *param3 = redis_create_bulk_string(value);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  redis_array_push_back(cmd, param3);
  return redis_send_command(cmd);
}

return_code redis_strlen(const char *key)
{
  redis_object *param1 = redis_create_bulk_string("STRLEN");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_command(cmd);
}