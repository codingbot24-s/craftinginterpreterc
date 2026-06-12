

#include "object.h"
#include <string.h>
#include "memory.h"
#include "value.h"
#include <stddef.h>
#include <stdio.h>

#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocate_object(sizeof(type), objectType)


static Obj* allocate_object(size_t size, ObjType type)
{
    Obj* object = (Obj*)reallocate(NULL,0,size);
    object->type = type;
    return object;    
}

static obj_string* allocate_string(char* chars,int len) 
{
    obj_string* string = ALLOCATE_OBJ(obj_string,OBJ_STRING);
    string->len = len;
    string->chars = chars;
    
    return string;
}


obj_string* copy_string (const char* chars, int len)
{
    char* heap_chars = ALLOCATE(char, len + 1);
    memcpy(heap_chars, chars, len);
    heap_chars[len] = '\0';
    return allocate_string(heap_chars,len);
};


void print_object(Value value) 
{
    switch (OBJ_TYPE(value))
    {
    case OBJ_STRING:
        printf("%s", AS_CSTRING(value));
      break;
        break;
    
    default:
        break;
    }
}



obj_string* take_string(char* chars,int len) 
{
    return allocate_string(chars,len);
}