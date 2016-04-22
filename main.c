#include "roku_redis_connect.h"
#include "roku_redis_write.h"
#include "roku_redis_read.h"
#include <stdio.h>
#include <string.h>

int main()
{
  return_code status;

  if((status = roku_redis_connect("127.0.0.1", 6666)) == SUCCESS)
    printf("Connected.\n");
  else
  {
    printf("Connection failed.\n");
    return 1;
  }

  const char *data = "SENDING STR TEST\n";
  if((status = roku_redis_write(data, strlen(data))) == SUCCESS)
    printf("Successfully sent: %s\n", data);

  char *buffer = NULL;
  if((status = roku_redis_read(&buffer)) == SUCCESS)
    printf("Received: %s\n", buffer);
}
