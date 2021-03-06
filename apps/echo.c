#include <stdio.h>
#include <string.h>
#include "redis_connection_commands.h"
#include "redis_connect.h"
#include "redis_io.h"

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
  
  redis_echo("hello world!");
  
  char *buffer = NULL;
  if((status = redis_read(&buffer)) == SUCCESS)
  {
    redis_object *response = redis_deserialize_object(buffer);
    printf("Received response:\n");
    redis_pretty_print_object(response);
  }
  
  return 0;
}
