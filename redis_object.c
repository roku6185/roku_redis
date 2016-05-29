#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "redis_object.h"
#include "redis_utils.h"

redis_object *redis_create_nil()
{
  redis_object *obj = malloc(sizeof(redis_object));
  obj->type = NIL;
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

redis_object *redis_create_bulk_string(const char *value)
{
  return redis_create_bulk_string_fixed_length(value, strlen(value));
}

redis_object *redis_create_bulk_string_fixed_length(const char *value, int length)
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

redis_serialization redis_serialize_object(redis_object *object)
{
  switch(object->type)
  {
  case NIL:
    {
      int length = 5 + 1;
      char *ret = malloc(length);
      snprintf(ret, length, "$-1\r\n");
      return (struct redis_serialization){ret, length};  
    }
    
  case INTEGER:
    {
      int length = 3 + object->length + 1;
      char *ret = malloc(length);
      snprintf(ret, length, ":%d\r\n", object->value.integer);
      return (struct redis_serialization){ret, length};
    }

  case STRING:
    {
      int length = 3 + object->length + 1;
      char *ret = malloc(length);
      snprintf(ret, length, "+%s\r\n", object->value.string);
      return (struct redis_serialization){ret, length};
    }
    
  case ERROR:
    {
      int length = 3 + object->length + 1;
      char *ret = malloc(length);
      snprintf(ret, length, "-%s\r\n", object->value.string);
      return (struct redis_serialization){ret, length};
    }

  case BULK_STRING:
    {
      int prefix_length = 1 + number_of_digits(object->length) + 2;
      int length = prefix_length + object->length + 2 + 1;
      char *ret = calloc(length, 1);
      snprintf(ret, length, "$%d\r\n", object->length);
      memcpy(ret + prefix_length, object->value.string, object->length);
      snprintf(ret + length - 3, 3, "\r\n");
      return (struct redis_serialization){ret, length};
    }

  case ARRAY:
    {
      int bytes = 0;
      int buffer_length = 1024;
      char *buffer = calloc(1024, 1);
      redis_object *tmp_obj = object->value.head;
      
      while(tmp_obj)
      {
        redis_serialization obj_serialized = redis_serialize_object(tmp_obj);
        bytes += obj_serialized.length;
        
        if(bytes > buffer_length)
        {
          char *tmp_ptr = realloc(buffer, buffer_length * 2);
          buffer_length *= 2;
          
          if(tmp_ptr)
            buffer = tmp_ptr;
        }
        
        strncat(buffer, obj_serialized.value, obj_serialized.length);
        tmp_obj = tmp_obj->next;
      }
      
      int ret_length = 3 + strlen(buffer) + number_of_digits(object->length) + 1;
      char *ret = calloc(ret_length, 1);
      snprintf(ret, ret_length, "*%d\r\n%s", object->length, buffer);
      free(buffer);
      return (struct redis_serialization){ret, ret_length};
    }
  }
}

int internal_deserialize_integer(const char **data)
{
  int value = 0;
  
  while(**data != '\r')
  {
    value = (value * 10) + (**data - '0');
    (*data)++;
  }
  
  (*data) += 2; // skip trailing \r\n
  return value;
}

char *internal_deserialize_string(const char **data)
{
  char *value = calloc(strlen(*data), 1);
  int i = 0;
  
  while(**data != '\r')
  {
    value[i++] = **data;
    (*data)++;
  }
  
  (*data) += 2; // skip trailing \r\n
  return value;
}

redis_object *redis_deserialize_object_internal(const char **data)
{
  switch(**data)
  {
    // INTEGER: :<integer>\r\n
    case ':':
      {
        (*data)++;
        int sign = 1;
        
        if(**data == '-')
        {
          sign = -1;
          (*data)++;
        }
        
        int value = sign * internal_deserialize_integer(data);
        return redis_create_integer(value);
      }
    
    // STRING: +<string>\r\n
    case '+':
      {
        (*data)++;
        char *value = internal_deserialize_string(data);
        return redis_create_string(value);
      }
      
    // ERROR: -<string>\r\n
    case '-':
      {
        (*data)++;
        char *value = internal_deserialize_string(data);
        return redis_create_error(value);
      }
    
    // BULK_STRING: $<integer>\r\n<string>\r\n
    case '$':
      {
        (*data)++;
        
        // Check for the null bulk string: $-1\r\n
        if(**data == '-')
        {
          (*data) += 4;
          return redis_create_nil();
        }
          
        int length = internal_deserialize_integer(data);
        char *value = internal_deserialize_string(data);
        return redis_create_bulk_string_fixed_length(value, length);
      }
    
    // ARRAY: *<integer>\r\n<entries>
    case '*':
      {
        (*data)++;
        
        // Check for the null array: *-1\r\n
        if(**data == '-')
        {
          (*data) += 4;
          return redis_create_nil();
        }
        
        int length = internal_deserialize_integer(data);
        redis_object *ret = redis_create_array();
        
        for(int i = 0; i < length; i++)
        {
          redis_object *tmp = redis_deserialize_object_internal(data);
          redis_array_push_back(ret, tmp);
        }
        
        return ret;
      }
  }
  
  return NULL;
}

redis_object *redis_deserialize_object(const char *data)
{
  return redis_deserialize_object_internal(&data);
}

void redis_free_object(redis_object *object)
{
  if(object == NULL)
    return;
    
  switch(object->type)
  {
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
    
  default:
    {
      free(object);
      return;
    }
  }
}

void redis_pretty_print_recursive(redis_object *object, int indent)
{
  switch(object->type)
  {
    case NIL:
      printf("(nil)\n");
      break;
      
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