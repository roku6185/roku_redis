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
  const char *field = "myfield";
  const char *value = "my test value";
  
  if(redis_hset(&response, key, field, value) == SUCCESS)
    printf("%s.%s=%s\n", key, field, value);
  
  if(redis_hexists(&response, key, field) == SUCCESS)
    printf("Exists %s.%s? %s\n", key, field, response > 0 ? "YES" : "NO");
  return 0;
}
