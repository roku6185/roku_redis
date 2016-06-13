#include <stdio.h>
#include <string.h>
#include "redis_hash_commands.h"
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
  const char *key = "mykey";
  const char *field1 = "myfield1";
  const char *field2 = "myfield2";
  const char *field3 = "myfield3";
  const char *value1 = "my test value 1";
  const char *value2 = "my test value 2";
  const char *value3 = "my test value 3";
  
  if(redis_hset(&response, key, field1, value1) == SUCCESS)
    printf("%s.%s=%s\n", key, field1, value1);
    
  if(redis_hset(&response, key, field2, value2) == SUCCESS)
    printf("%s.%s=%s\n", key, field2, value2);
    
  if(redis_hset(&response, key, field3, value3) == SUCCESS)
    printf("%s.%s=%s\n", key, field3, value3);
    
  if(redis_hdel(&response, key, 1, field1) == SUCCESS)
    printf("%d fields deleted\n", response);
    
  if(redis_hdel(&response, key, 2, field2, field3) == SUCCESS)
    printf("%d fields deleted\n", response);
  return 0;
}
