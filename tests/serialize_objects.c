#include <stdlib.h>
#include <unity/unity.h>
#include "redis_object.h"

void test_serialize_nil()
{
  redis_object *obj = redis_create_nil();
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "$-1\r\n");
  redis_free_object(obj);
}

void test_serialize_integer1()
{
  int value = 0;
  redis_object *obj = redis_create_integer(value);
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, ":0\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_integer2()
{
  int value = 123;
  redis_object *obj = redis_create_integer(value);
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, ":123\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_integer3()
{
  int value = -123;
  redis_object *obj = redis_create_integer(value);
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, ":-123\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_string1()
{
  const char *str = "";
  redis_object *obj = redis_create_string(str);
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "+\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_string2()
{
  const char *str = "testing";
  redis_object *obj = redis_create_string(str);
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "+testing\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_string3()
{
  const char *str = "testing spaces";
  redis_object *obj = redis_create_string(str);
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "+testing spaces\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_error1()
{
  const char *str = "";
  redis_object *obj = redis_create_error(str);
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "-\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_error2()
{
  const char *str = "testing";
  redis_object *obj = redis_create_error(str);
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "-testing\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_error3()
{
  const char *str = "testing spaces";
  redis_object *obj = redis_create_error(str);
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "-testing spaces\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_bulk_string1()
{
  const char *str = "";
  redis_object *obj = redis_create_bulk_string(str, 0);
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "$0\r\n\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_bulk_string2()
{
  const char *str = "testing";
  redis_object *obj = redis_create_bulk_string(str, 7);
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "$7\r\ntesting\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_bulk_string3()
{
  const char *str = "testing spaces";
  redis_object *obj = redis_create_bulk_string(str, 14);
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "$14\r\ntesting spaces\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_array1()
{
  redis_object *obj = redis_create_array();
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "*0\r\n");
  redis_free_object(obj);
  free((char*)tmp);
}

void test_serialize_array2()
{
  redis_object *obj = redis_create_array();
  redis_array_push_back(obj, redis_create_integer(1));
  redis_array_push_back(obj, redis_create_integer(2));
  redis_array_push_back(obj, redis_create_integer(3));
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "*3\r\n:1\r\n:2\r\n:3\r\n");
  redis_free_object(obj);
  free((char*)tmp);
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
  const char *tmp = redis_serialize_object(obj);
  TEST_ASSERT_EQUAL_STRING(tmp, "*2\r\n*3\r\n:123\r\n+test one\r\n+test two\r\n*2\r\n:456\r\n+test three\r\n");
  redis_free_object(obj);
  free((char*)tmp);
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
  RUN_TEST(test_serialize_array1);
  RUN_TEST(test_serialize_array2);
  RUN_TEST(test_serialize_array3);
  return UNITY_END();
}