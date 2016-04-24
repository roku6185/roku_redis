#include "roku_redis_object.h"
#include "roku_redis_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

redis_object *redis_create_nil()
{
  redis_object *obj = malloc(sizeof(redis_object));
  obj->type = NIL;
  obj->next = NULL;
  return obj;
}

redis_object *redis_create_nil_array()
{
  redis_object *obj = malloc(sizeof(redis_object));
  obj->type = NIL_ARRAY;
  obj->next = NULL;
  return obj;
}

redis_object *redis_create_integer(int value)
{
  redis_object *obj = malloc(sizeof(redis_object));
  obj->type = INTEGER;
  obj->value.integer = value;
  obj->length = number_of_digits(value);
  obj->next = NULL;
  return obj;
}

redis_object *redis_create_string(const char *value)
{
  redis_object *obj = malloc(sizeof(redis_object));
  obj->type = STRING;
  obj->value.string = value;
  obj->length = strlen(value);
  obj->next = NULL;
  return obj;
}

redis_object *redis_create_bulk_string(const char *value, int length)
{
  redis_object *obj = malloc(sizeof(redis_object));
  obj->type = BULK_STRING;
  obj->value.string = value;
  obj->length = length;
  obj->next = NULL;
  return obj;
}

redis_object *redis_create_error(const char *value)
{
  redis_object *obj = malloc(sizeof(redis_object));
  obj->type = ERROR;
  obj->value.string = value;
  obj->length = strlen(value);
  obj->next = NULL;
  return obj;
}

redis_object *redis_create_array()
{
  redis_object *obj = malloc(sizeof(redis_object));
  obj->type = ARRAY;
  obj->value.head = NULL;
  obj->length = 0;
  obj->next = NULL;
  return obj;
}

void redis_array_push_back(redis_object *parent, redis_object *child)
{
  if(parent->value.head == NULL)
  {
    parent->value.head = child;
    parent->length = 1;
    return;
  }
  
  redis_object *current = parent->value.head;
  
  while(current)
  {
    if(current->next == NULL)
      break;
    
    current = current->next;
  }
  
  current->next = child;
  parent->length++;
}

const char *redis_serialize_object(redis_object *object)
{
  switch(object->type)
  {
  case NIL:
    {
      return "$-1\r\n";  
    }
    
  case NIL_ARRAY:
    {
      return "*-1\r\n";  
    }
    
  case INTEGER:
    {
      int length = 3 + object->length;
      char *ret = malloc(length);
      snprintf(ret, length, ":%d\r\n", object->value.integer);
      return ret;
    }

  case STRING:
    {
      int length = 3 + object->length;
      char *ret = malloc(length);
      snprintf(ret, length, "+%s\r\n", object->value.string);
      return ret;
    }
    
  case ERROR:
    {
      int length = 3 + object->length;
      char *ret = malloc(length);
      snprintf(ret, length, "-%s\r\n", object->value.string);
      return ret;
    }

  case BULK_STRING:
    {
      int length = 5 + object->length + 1;
      char *ret = malloc(length);
      snprintf(ret, length, "$%d\r\n%s\r\n", object->length, object->value.string);
      return ret;
    }

  case ARRAY:
    {
      int bytes = 0;
      int buffer_length = 1024;
      char *buffer = calloc(1024, 1);
      redis_object *tmp_obj = object->value.head;
      
      while(tmp_obj)
      {
        const char *tmp = redis_serialize_object(tmp_obj);
        bytes += strlen(tmp);
        
        if(bytes > buffer_length)
        {
          char *tmp_ptr = realloc(buffer, buffer_length * 2);
          buffer_length *= 2;
          
          if(tmp_ptr)
            buffer = tmp_ptr;
        }
        
        strncat(buffer, tmp, strlen(tmp));
        tmp_obj = tmp_obj->next;
      }
      
      int ret_length = strlen(buffer) + 3 + number_of_digits(object->length);
      char *ret = calloc(ret_length, 1);
      snprintf(ret, ret_length, "*%d\r\n%s", object->length, buffer);
      free(buffer);
      return ret;
    }
  }
}

redis_object *redis_deserialize_object(const char *data)
{
  int length = strlen(data);
  
  if(length <= 0)
    return NULL;
  
  switch(data[0])
  {
    // INTEGER
    case ':':
      {
        int value = 0;
        
        if(sscanf(data, ":%d\r\n", &value) != 1)
          return NULL;
          
        return redis_create_integer(value);
      }
    
    // STRING
    case '+':
      {
        char *value = malloc(strlen(data));
        
        if(sscanf(data, "+%s\r\n", value) != 1)
          return NULL;
          
        return redis_create_string(value);
      }
      
    // ERROR
    case '-':
      {
        char *value = malloc(strlen(data));
        
        if(sscanf(data, "-%s\r\n", value) != 1)
          return NULL;
          
        return redis_create_error(value);
      }
    
    // BULK_STRING
    case '$':
      {
        int length = 0;
        char *value = malloc(strlen(data));
        
        // Check for the null bulk string
        if(sscanf(data, "$-%d\r\n", &length) == 1 && length == 1)
          return redis_create_nil();
        
        if(sscanf(data, "$%d\r\n%s\r\n", &length, value) != 2)
          return NULL;
          
        return redis_create_bulk_string(value, length);
      }
    
    // ARRAY  
    case '*':
      {
        int length = 0;
        char *value = malloc(strlen(data));
        
        // Check for the null array
        if(sscanf(data, "*-%d\r\n", &length) == 1 && length == 1)
          return redis_create_nil_array();
        
        if(sscanf(data, "*%d\r\n%s", &length, value) != 2)
          return NULL;
        
        redis_deserialize_object(value);
        //redis_create_array();
      }
      break;
  }
  
  return NULL;
}

void redis_free_object(redis_object *object)
{
  if(object == NULL)
    return;
    
  switch(object->type)
  {
  case STRING:
  case BULK_STRING:
  case ERROR:
  case NIL:
    {
      //free((char *)object->value.string);
      free(object);
      return;
    }

  case ARRAY:
    {
      redis_object *parent = object;
      object = object->value.head;
      
      while(object)
      {
        redis_object *tmp = object->next;
        redis_free_object(object);
        object = tmp;
      }
      
      free(parent);
      return;
    }
  }
}

void redis_pretty_print_recursive(redis_object *object, int indent)
{
  switch(object->type)
  {
    case INTEGER:
      printf("%d (integer)\n", object->value.integer);
      break;
      
    case STRING:
      printf("%s (string)\n", object->value.string);
      break;
      
    case BULK_STRING:
      printf("%s (bulk string)\n", object->value.string);
      break;
      
    case ERROR:
      printf("%s (error)\n", object->value.string);
      break;
    
    case ARRAY:
    {
      printf("array (size=%d)\n", object->length);
      redis_object *tmp = object->value.head;
      int i = 0;
      
      while(tmp)
      {
        printf("%*s[%d]: ", indent, " ", ++i);
        redis_pretty_print_recursive(tmp, indent * 2);
        tmp = tmp->next;
      }
      break;
    }
  }
}

void redis_pretty_print_object(redis_object *object)
{
  redis_pretty_print_recursive(object, 2);
}