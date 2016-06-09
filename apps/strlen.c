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
  
  int ret = 0;
  ret = redis_strlen("testkey", &status);
  printf("Strlen=%d\n", ret);
  return 0;
}
