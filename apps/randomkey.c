#include <stdio.h>
#include <string.h>
#include "redis_key_commands.h"
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
  
  redis_object *response = NULL;
  
  if(redis_randomkey(&response) == SUCCESS)
    redis_pretty_print_object(response);
  
  redis_free_object(response);
  return 0;
}
