#include "../roku_redis_object.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

int main()
{  
  redis_object *obj1 = redis_create_integer(12345);
  const char *str1 = redis_serialize_object(obj1);
  printf("%s\n", str1);
  redis_object *obj1_dup = redis_deserialize_object(str1);
  redis_pretty_print_object(obj1_dup);
  free((char *)str1);
  redis_free_object(obj1);
  
  redis_object *obj2 = redis_create_string("testing string");
  const char *str2 = redis_serialize_object(obj2);
  printf("%s\n", str2);
  redis_free_object(obj2);
  
  redis_object *obj2b = redis_create_bulk_string("bulk str", 8);
  const char *str2b = redis_serialize_object(obj2b);
  printf("%s\n", str2b);
  redis_free_object(obj2b);
  
  redis_object *obj3 = redis_create_error("error message");
  const char *str3 = redis_serialize_object(obj3);
  printf("%s\n", str3);
  free((char *)str3);
  redis_free_object(obj3);
  
  redis_object *obj4 = redis_create_array();
  const char *str4 = redis_serialize_object(obj4);
  printf("%s\n", str4);
  free((char *)str4);
  redis_free_object(obj4);
  
  redis_object *obj5 = redis_create_array();
  redis_array_push_back(obj5, redis_create_integer(789));
  redis_array_push_back(obj5, redis_create_string("array string test"));
  const char *str5 = redis_serialize_object(obj5);
  printf("%s\n", str5);
  free((char *)str5);
  redis_free_object(obj5);
  
  // "complex" array test
  redis_object *obj6_arr1 = redis_create_array();
  redis_array_push_back(obj6_arr1, redis_create_integer(456));
  redis_array_push_back(obj6_arr1, redis_create_string("string one"));
  
  redis_object *obj6_arr2 = redis_create_array();
  redis_array_push_back(obj6_arr2, redis_create_integer(321));
  redis_array_push_back(obj6_arr2, redis_create_string("string two"));
  
  redis_object *obj6 = redis_create_array();
  redis_array_push_back(obj6, redis_create_string("string start"));
  redis_array_push_back(obj6, obj6_arr1);
  redis_array_push_back(obj6, obj6_arr2);
  
  redis_pretty_print_object(obj6);
  
  const char *str6 = redis_serialize_object(obj6);
  printf("%s\n", str6);
  
  redis_object *obj6_dup = redis_deserialize_object(str6);
  redis_pretty_print_object(obj6_dup);
  free((char *)str6);
  redis_free_object(obj6);
  
  return 0;
}
