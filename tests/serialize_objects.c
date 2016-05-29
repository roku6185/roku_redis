#include <stdlib.h>
#include <unity/unity.h>
#include "redis_object.h"

void test_serialize_nil()
{
  redis_object *obj = redis_create_nil();
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "$-1\r\n");
  redis_free_object(obj);
}

void test_serialize_integer1()
{
  int value = 0;
  redis_object *obj = redis_create_integer(value);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, ":0\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_integer2()
{
  int value = 123;
  redis_object *obj = redis_create_integer(value);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, ":123\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_integer3()
{
  int value = -123;
  redis_object *obj = redis_create_integer(value);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, ":-123\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_string1()
{
  const char *str = "";
  redis_object *obj = redis_create_string(str);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "+\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_string2()
{
  const char *str = "testing";
  redis_object *obj = redis_create_string(str);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "+testing\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_string3()
{
  const char *str = "testing spaces";
  redis_object *obj = redis_create_string(str);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "+testing spaces\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_error1()
{
  const char *str = "";
  redis_object *obj = redis_create_error(str);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "-\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_error2()
{
  const char *str = "testing";
  redis_object *obj = redis_create_error(str);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "-testing\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_error3()
{
  const char *str = "testing spaces";
  redis_object *obj = redis_create_error(str);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "-testing spaces\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_bulk_string1()
{
  const char *str = "";
  redis_object *obj = redis_create_bulk_string_fixed_length(str, 0);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "$0\r\n\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_bulk_string2()
{
  const char *str = "testing";
  redis_object *obj = redis_create_bulk_string(str);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "$7\r\ntesting\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_bulk_string3()
{
  const char *str = "testing spaces";
  redis_object *obj = redis_create_bulk_string(str);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "$14\r\ntesting spaces\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_bulk_string4()
{
  const char *str = "null \0 byte";
  redis_object *obj = redis_create_bulk_string_fixed_length(str, 11);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  const char *expected = "$11\r\nnull \0 byte\r\n";
  TEST_ASSERT_EQUAL_MEMORY(obj_serialized.value, expected, obj_serialized.length + 1);
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_array1()
{
  redis_object *obj = redis_create_array();
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "*0\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_array2()
{
  redis_object *obj = redis_create_array();
  redis_array_push_back(obj, redis_create_integer(1));
  redis_array_push_back(obj, redis_create_integer(2));
  redis_array_push_back(obj, redis_create_integer(3));
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "*3\r\n:1\r\n:2\r\n:3\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}

void test_serialize_array3()
{
  redis_object *obj = redis_create_array();
  redis_object *tmp1 = redis_create_array();
  redis_array_push_back(tmp1, redis_create_integer(123));
  redis_array_push_back(tmp1, redis_create_string("test one"));
  redis_array_push_back(tmp1, redis_create_string("test two"));
  redis_object *tmp2 = redis_create_array();
  redis_array_push_back(tmp2, redis_create_integer(456));
  redis_array_push_back(tmp2, redis_create_string("test three"));
  redis_array_push_back(obj, tmp1);
  redis_array_push_back(obj, tmp2);
  redis_serialization obj_serialized = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(obj_serialized.value, "*2\r\n*3\r\n:123\r\n+test one\r\n+test two\r\n*2\r\n:456\r\n+test three\r\n");
  redis_free_object(obj);
  free((char*)obj_serialized.value);
}


int main()
{
  UNITY_BEGIN();
  RUN_TEST(test_serialize_nil);
  RUN_TEST(test_serialize_integer1);
  RUN_TEST(test_serialize_integer2);
  RUN_TEST(test_serialize_integer3);
  RUN_TEST(test_serialize_string1);
  RUN_TEST(test_serialize_string2);
  RUN_TEST(test_serialize_string3);
  RUN_TEST(test_serialize_error1);
  RUN_TEST(test_serialize_error2);
  RUN_TEST(test_serialize_error3);
  RUN_TEST(test_serialize_bulk_string1);
  RUN_TEST(test_serialize_bulk_string2);
  RUN_TEST(test_serialize_bulk_string3);
  RUN_TEST(test_serialize_bulk_string4);
  RUN_TEST(test_serialize_array1);
  RUN_TEST(test_serialize_array2);
  RUN_TEST(test_serialize_array3);
  return UNITY_END();
}