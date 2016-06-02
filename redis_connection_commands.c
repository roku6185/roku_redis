#include <string.h>
#include "redis_write.h"
#include "redis_return_codes.h"

return_code redis_echo(const char *message)
{
  redis_object *param1 = redis_create_bulk_string("ECHO");
  redis_object *param2 = redis_create_bulk_string(message);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_command(cmd);
}

return_code redis_ping(const char *message)
{
  redis_object *param1 = redis_create_bulk_string("PING");
  redis_object *param2 = redis_create_bulk_string(message);
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  return redis_send_command(cmd);
}

return_code redis_quit()
{
  redis_object *param1 = redis_create_bulk_string("QUIT");
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  return redis_send_command(cmd);
}