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
  
  int response = -1;
  const char *key = "testkey";
  
  if(redis_expire(&response, key, 120) == SUCCESS)
    printf("Timeout set for %s? %s\n", key, response > 0 ? "YES" : "NO");
  return 0;
}
