#include <stdio.h>
#include <string.h>
#include "redis_string_commands.h"
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
  
  bool ret = redis_set("testkey", "my value in key", &status);
  printf("Response: %s\n", ret == TRUE ? "OK" : "FAIL");
  return 0;
}
