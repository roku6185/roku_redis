#include <stdlib.h>
#include <unity/unity.h>
#include "redis_object.h"

void test_deserialize_nil1()
{
  redis_object *obj = redis_deserialize_object("$-1\r\n");
  TEST_ASSERT_EQUAL(obj->type, NIL);
  redis_free_object(obj);
}

void test_deserialize_nil2()
{
  redis_object *obj = redis_deserialize_object("*-1\r\n");
  TEST_ASSERT_EQUAL(obj->type, NIL);
  redis_free_object(obj);
}

void test_deserialize_integer1()
{
  redis_object *obj = redis_deserialize_object(":0\r\n");
  TEST_ASSERT_EQUAL(obj->type, INTEGER);
  TEST_ASSERT_EQUAL(obj->length, 1);
  TEST_ASSERT_EQUAL(obj->value.integer, 0);
  redis_free_object(obj);
}

void test_deserialize_integer2()
{
  redis_object *obj = redis_deserialize_object(":123\r\n");
  TEST_ASSERT_EQUAL(obj->type, INTEGER);
  TEST_ASSERT_EQUAL(obj->length, 3);
  TEST_ASSERT_EQUAL(obj->value.integer, 123);
  redis_free_object(obj);
}

void test_deserialize_integer3()
{
  redis_object *obj = redis_deserialize_object(":-123\r\n");
  TEST_ASSERT_EQUAL(obj->type, INTEGER);
  TEST_ASSERT_EQUAL(obj->length, 4);
  TEST_ASSERT_EQUAL(obj->value.integer, -123);
  redis_free_object(obj);
}

void test_deserialize_string1()
{
  redis_object *obj = redis_deserialize_object("+\r\n");
  TEST_ASSERT_EQUAL(obj->type, STRING);
  TEST_ASSERT_EQUAL(obj->length, 0);
  TEST_ASSERT_EQUAL_STRING(obj->value.string, "");
  redis_free_object(obj);
}

void test_deserialize_string2()
{
  redis_object *obj = redis_deserialize_object("+testing\r\n");
  TEST_ASSERT_EQUAL(obj->type, STRING);
  TEST_ASSERT_EQUAL(obj->length, 7);
  TEST_ASSERT_EQUAL_STRING(obj->value.string, "testing");
  redis_free_object(obj);
}

void test_deserialize_string3()
{
  redis_object *obj = redis_deserialize_object("+testing spaces\r\n");
  TEST_ASSERT_EQUAL(obj->type, STRING);
  TEST_ASSERT_EQUAL(obj->length, 14);
  TEST_ASSERT_EQUAL_STRING(obj->value.string, "testing spaces");
  redis_free_object(obj);
}

void test_deserialize_error1()
{
  redis_object *obj = redis_deserialize_object("-\r\n");
  TEST_ASSERT_EQUAL(obj->type, ERROR);
  TEST_ASSERT_EQUAL(obj->length, 0);
  TEST_ASSERT_EQUAL_STRING(obj->value.string, "");
  redis_free_object(obj);
}

void test_deserialize_error2()
{
  redis_object *obj = redis_deserialize_object("-testing\r\n");
  TEST_ASSERT_EQUAL(obj->type, ERROR);
  TEST_ASSERT_EQUAL(obj->length, 7);
  TEST_ASSERT_EQUAL_STRING(obj->value.string, "testing");
  redis_free_object(obj);
}

void test_deserialize_error3()
{
  redis_object *obj = redis_deserialize_object("-testing spaces\r\n");
  TEST_ASSERT_EQUAL(obj->type, ERROR);
  TEST_ASSERT_EQUAL(obj->length, 14);
  TEST_ASSERT_EQUAL_STRING(obj->value.string, "testing spaces");
  redis_free_object(obj);
}

void test_deserialize_bulk_string1()
{
  redis_object *obj = redis_deserialize_object("$0\rn\r\n");
  TEST_ASSERT_EQUAL(obj->type, BULK_STRING);
  TEST_ASSERT_EQUAL(obj->length, 0);
  TEST_ASSERT_EQUAL_STRING(obj->value.string, "");
  redis_free_object(obj);
}

void test_deserialize_bulk_string2()
{
  redis_object *obj = redis_deserialize_object("$7\r\ntesting\r\n");
  TEST_ASSERT_EQUAL(obj->type, BULK_STRING);
  TEST_ASSERT_EQUAL(obj->length, 7);
  TEST_ASSERT_EQUAL_STRING(obj->value.string, "testing");
  redis_free_object(obj);
}

void test_deserialize_bulk_string3()
{
  redis_object *obj = redis_deserialize_object("$14\r\ntesting spaces\r\n");
  TEST_ASSERT_EQUAL(obj->type, BULK_STRING);
  TEST_ASSERT_EQUAL(obj->length, 14);
  TEST_ASSERT_EQUAL_STRING(obj->value.string, "testing spaces");
  redis_free_object(obj);
}

void test_deserialize_array1()
{
  redis_object *obj = redis_deserialize_object("*0\r\n");
  TEST_ASSERT_EQUAL(obj->type, ARRAY);
  TEST_ASSERT_EQUAL(obj->length, 0);
  redis_free_object(obj);
}

void test_deserialize_array2()
{
  redis_object *obj = redis_deserialize_object("*3\r\n:1\r\n:2\r\n:3\r\n");
  TEST_ASSERT_EQUAL(obj->type, ARRAY);
  TEST_ASSERT_EQUAL(obj->length, 3);
  
  // integer 1
  redis_object *tmp = obj->value.head;
  TEST_ASSERT_NOT_EQUAL(tmp, NULL);
  TEST_ASSERT_EQUAL(tmp->type, INTEGER);
  TEST_ASSERT_EQUAL(tmp->value.integer, 1);
  
  // integer 2
  tmp = tmp->next;
  TEST_ASSERT_NOT_EQUAL(tmp, NULL);
  TEST_ASSERT_EQUAL(tmp->type, INTEGER);
  TEST_ASSERT_EQUAL(tmp->value.integer, 2);
  
  // integer 3
  tmp = tmp->next;
  TEST_ASSERT_NOT_EQUAL(tmp, NULL);
  TEST_ASSERT_EQUAL(tmp->type, INTEGER);
  TEST_ASSERT_EQUAL(tmp->value.integer, 3);
  
  redis_free_object(obj);
}

void test_deserialize_array3()
{
  redis_object *obj = redis_deserialize_object("*2\r\n*3\r\n:123\r\n+test one\r\n+test two\r\n*2\r\n:456\r\n+test three\r\n");
  TEST_ASSERT_EQUAL(obj->type, ARRAY);
  TEST_ASSERT_EQUAL(obj->length, 2);
  
  // array 1
  redis_object *tmp = obj->value.head;
  TEST_ASSERT_NOT_EQUAL(tmp, NULL);
  TEST_ASSERT_EQUAL(tmp->type, ARRAY);
  TEST_ASSERT_EQUAL(tmp->length, 3);
  
  tmp = tmp->value.head;
  TEST_ASSERT_NOT_EQUAL(tmp, NULL);
  TEST_ASSERT_EQUAL(tmp->type, INTEGER);
  TEST_ASSERT_EQUAL(tmp->value.integer, 123);
  
  tmp = tmp->next;
  TEST_ASSERT_NOT_EQUAL(tmp, NULL);
  TEST_ASSERT_EQUAL(tmp->type, STRING);
  TEST_ASSERT_EQUAL_STRING(tmp->value.string, "test one");
  
  tmp = tmp->next;
  TEST_ASSERT_NOT_EQUAL(tmp, NULL);
  TEST_ASSERT_EQUAL(tmp->type, STRING);
  TEST_ASSERT_EQUAL_STRING(tmp->value.string, "test two");
  
  // array 2
  tmp = obj->value.head->next;
  TEST_ASSERT_NOT_EQUAL(tmp, NULL);
  TEST_ASSERT_EQUAL(tmp->type, ARRAY);
  TEST_ASSERT_EQUAL(tmp->length, 2);
  
  tmp = tmp->value.head;
  TEST_ASSERT_NOT_EQUAL(tmp, NULL);
  TEST_ASSERT_EQUAL(tmp->type, INTEGER);
  TEST_ASSERT_EQUAL(tmp->value.integer, 456);
  
  tmp = tmp->next;
  TEST_ASSERT_NOT_EQUAL(tmp, NULL);
  TEST_ASSERT_EQUAL(tmp->type, STRING);
  TEST_ASSERT_EQUAL_STRING(tmp->value.string, "test three");
  
  redis_free_object(obj);
}


int main()
{
  UNITY_BEGIN();
  RUN_TEST(test_deserialize_nil1);
  RUN_TEST(test_deserialize_nil2);
  RUN_TEST(test_deserialize_integer1);
  RUN_TEST(test_deserialize_integer2);
  RUN_TEST(test_deserialize_integer3);
  RUN_TEST(test_deserialize_string1);
  RUN_TEST(test_deserialize_string2);
  RUN_TEST(test_deserialize_string3);
  RUN_TEST(test_deserialize_error1);
  RUN_TEST(test_deserialize_error2);
  RUN_TEST(test_deserialize_error3);
  RUN_TEST(test_deserialize_bulk_string1);
  RUN_TEST(test_deserialize_bulk_string2);
  RUN_TEST(test_deserialize_bulk_string3);
  RUN_TEST(test_deserialize_array1);
  RUN_TEST(test_deserialize_array2);
  RUN_TEST(test_deserialize_array3);
  return UNITY_END();
}