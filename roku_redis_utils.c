#include <stdio.h>
#include <string.h>

int number_of_digits(int value)
{
  char tmp[256];
  snprintf(tmp, 256, "%d", value);
  return strlen(tmp);
}