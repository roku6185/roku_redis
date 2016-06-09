#include <stdio.h>
#include <string.h>
#include "redis_io.h"
#include "redis_return_codes.h"

int redis_append(const char *key, const char *value, return_code *status)
{
  redis_object *param1 = redis_create_bulk_string("APPEND");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *param3 = redis_create_bulk_string(value);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  redis_array_push_back(cmd, param3);
  
  redis_object *obj = redis_send_and_receive_command(cmd, status);
  
  if(*status == SUCCESS && obj)
    return redis_object_to_integer(obj, status);
  
  return -1;
}

redis_object *redis_get(const char *key, return_code *status)
{
  redis_object *param1 = redis_create_bulk_string("GET");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  
  redis_object *obj = redis_send_and_receive_command(cmd, status);
  
  if(*status == SUCCESS)
    return obj;
  
  return NULL;
}

bool redis_set(const char *key, const char *value, return_code *status)
{
  redis_object *param1 = redis_create_bulk_string("SET");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *param3 = redis_create_bulk_string(value);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  redis_array_push_back(cmd, param3);
  
  redis_object *obj = redis_send_and_receive_command(cmd, status);
  
  if(*status == SUCCESS && obj)
  {
    const char *reply = redis_object_to_string(obj, status);
    
    if(reply && (strncmp(reply, "OK", 2) == 0))
      return TRUE;
  }
  
  return FALSE;
}

int redis_strlen(const char *key, return_code *status)
{
  redis_object *param1 = redis_create_bulk_string("STRLEN");
  redis_object *param2 = redis_create_bulk_string(key);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  
  redis_object *obj = redis_send_and_receive_command(cmd, status);
  
  if(*status == SUCCESS && obj)
    return redis_object_to_integer(obj, status);
  
  return -1;
}