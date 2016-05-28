#include <stdio.h>
#include <string.h>
#include "redis_connect.h"
#include "redis_write.h"
#include "redis_read.h"
#include "redis_object.h"

int main()
{
  return_code status;

  if((status = redis_connect("127.0.0.1", "6379")) == SUCCESS)
    printf("Connected.\n");
  else
  {
    printf("Connection failed.\n");
    return 1;
  }
  
  redis_object *param1 = redis_create_bulk_string("GET");
  redis_object *param2 = redis_create_bulk_string("mykey");
  redis_object *cmd = redis_create_array();
  redis_array_push_back(cmd, param1);
  redis_array_push_back(cmd, param2);
  
  if((status = redis_send_command(cmd)) == SUCCESS)
    printf("Command was sent successfully.\n");
  else
  {
    printf("Sending command failed.\n");
    return 1;
  }
  
  char *buffer = NULL;
  if((status = redis_read(&buffer)) == SUCCESS)
  {
    redis_object *response = redis_deserialize_object(buffer);
    printf("Received response:\n");
    redis_pretty_print_object(response);
  }
  
  return 0;
}
