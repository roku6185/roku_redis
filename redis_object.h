#ifndef _REDIS_DATA_TYPE_H_
#define _REDIS_DATA_TYPE_H_

#include "redis_return_codes.h"

typedef struct redis_object redis_object;

typedef enum
{
  INTEGER,
  STRING,
  BULK_STRING,
  ARRAY,
  ERROR,
  NIL,
} redis_object_type;

typedef union
{
  int integer;
  const char *string;
  redis_object *head;
} redis_object_value;

typedef struct redis_object
{
  redis_object_type type;
  redis_object_value value;
  redis_object *next;
  int length;
} redis_object;

typedef struct redis_serialization
{
  const char *value;
  int length;
} redis_serialization;

redis_object *redis_create_nil();
redis_object *redis_create_integer(int value);
redis_object *redis_create_string(const char *value);
redis_object *redis_create_bulk_string(const char *value);
redis_object *redis_create_bulk_string_fixed_length(const char *value, int length);
redis_object *redis_create_error(const char *value);
redis_object *redis_create_array();
void redis_array_push_back(redis_object *parent, redis_object *child);

void redis_pretty_print_object(redis_object *object);
redis_serialization redis_serialize_object(redis_object *object);
redis_object *redis_deserialize_object(const char *data);
void redis_free_object(redis_object *object);

return_code redis_object_to_integer(redis_object *object, int *result);
return_code redis_object_to_string(redis_object *object, const char **result);

#endif