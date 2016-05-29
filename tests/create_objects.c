#include <unity/unity.h>
#include "redis_object.h"

void test_create_nil()
{
  redis_object *obj = redis_create_nil();
  TEST_ASSERT_EQUAL(obj->type, NIL);
  redis_free_object(obj);
}

void test_create_integer()
{
  int value = 123456;
  redis_object *obj = redis_create_integer(value);
  TEST_ASSERT_EQUAL(obj->type, INTEGER);
  TEST_ASSERT_EQUAL(obj->length, 6);
  TEST_ASSERT_EQUAL(obj->value.integer, value);
  redis_free_object(obj);
}

void test_create_string()
{
  const char *str = "testing";
  redis_object *obj = redis_create_string(str);
  TEST_ASSERT_EQUAL(obj->type, STRING);
  TEST_ASSERT_EQUAL(obj->length, 7);
  TEST_ASSERT_EQUAL(obj->value.string, str);
  redis_free_object(obj);
}

void test_create_error()
{
  const char *str = "testing";
  redis_object *obj = redis_create_error(str);
  TEST_ASSERT_EQUAL(obj->type, ERROR);
  TEST_ASSERT_EQUAL(obj->length, 7);
  TEST_ASSERT_EQUAL(obj->value.string, str);
  redis_free_object(obj);
}

void test_create_bulk_string()
{
  const char *str = "testing";
  redis_object *obj = redis_create_bulk_string(str);
  TEST_ASSERT_EQUAL(obj->type, BULK_STRING);
  TEST_ASSERT_EQUAL(obj->length, 7);
  TEST_ASSERT_EQUAL(obj->value.string, str);
  redis_free_object(obj);
}

void test_create_bulk_string_fixed_length()
{
  const char *str = "null \0 byte";
  int length = 11;
  redis_object *obj = redis_create_bulk_string_fixed_length(str, length);
  TEST_ASSERT_EQUAL(obj->type, BULK_STRING);
  TEST_ASSERT_EQUAL(obj->length, length);
  TEST_ASSERT_EQUAL_MEMORY(obj->value.string, str, 12);
  redis_free_object(obj);
}

void test_create_array()
{
  redis_object *obj = redis_create_array();
  redis_array_push_back(obj, redis_create_integer(1));
  redis_array_push_back(obj, redis_create_integer(2));
  redis_array_push_back(obj, redis_create_integer(3));
  TEST_ASSERT_EQUAL(obj->type, ARRAY);
  TEST_ASSERT_EQUAL(obj->length, 3);
  redis_free_object(obj);
}

int main()
{
  UNITY_BEGIN();
  RUN_TEST(test_create_nil);
  RUN_TEST(test_create_integer);
  RUN_TEST(test_create_string);
  RUN_TEST(test_create_error);
  RUN_TEST(test_create_bulk_string);
  RUN_TEST(test_create_bulk_string_fixed_length);
  RUN_TEST(test_create_array);
  return UNITY_END();
}
