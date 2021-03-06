#include <stdio.h>
#include <string.h>
#include <time.h>
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
  
  // Generate key based on timestamp
  time_t now = time(NULL);
  char key[32];
  sprintf(key, "%ld", now);
  printf("Using key=%s\n", key);
  
  int response;
  
  const char *value1 = "abc";
  if(redis_append(key, value1, &response) == SUCCESS)
    printf("Appended=%s: %s\n", value1, response == strlen(value1) ? "OK" : "FAIL");
  
  const char *value2 = "defghji";
  if(redis_append(key, value2, &response) == SUCCESS)
    printf("Appended=%s: %s\n", value2, response == strlen(value1) + strlen(value2) ? "OK" : "FAIL");
    
  return 0;
}