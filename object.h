#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

typedef enum
{
  OBJ_STRING,
} ObjType;

struct Obj
{
  ObjType type;
};

typedef struct obj_string 
{
  Obj obj;
  int len;
  char *chars;
}obj_string;

obj_string *copy_string(const char *chars, int len);
void print_object(Value value);

obj_string* take_string(char* chars, int len);

static inline bool is_obj_type(Value value, ObjType type)
{
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}
#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) is_obj_type(value, OBJ_STRING)

#define AS_STRING(value) ((obj_string*)AS_OBJ(value))
#define AS_CSTRING(value) (((obj_string *)AS_OBJ(value))->chars)

#endif // ! clox_object_h
