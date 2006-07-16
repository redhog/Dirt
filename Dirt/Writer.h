#ifndef DIRT_WRITER
#define DIRT_WRITER
#include <Dirt/WriteBuffer.h>

typedef struct Dirt_WriterT Dirt_Writer;

typedef char  (*Dirt_Writer_Write)(Dirt_Writer *writer, void *any);
typedef void *(*Dirt_Writer_Structure_Begin)(Dirt_Writer *writer, void *structure);
typedef char  (*Dirt_Writer_Structure_End)(Dirt_Writer *writer, void *iter);
typedef void *(*Dirt_Writer_Structure_Next)(Dirt_Writer *writer, void *iter);
typedef void *(*Dirt_Writer_Keyvalue_Key)(Dirt_Writer *writer, void *keyvalue);
typedef void *(*Dirt_Writer_Keyvalue_Value)(Dirt_Writer *writer, void *keyvalue);

typedef struct {
 Dirt_Writer_Write write;
 Dirt_Writer_Structure_Begin structure_begin;
 Dirt_Writer_Structure_End structure_end;
 Dirt_Writer_Structure_Next structure_next;
 Dirt_Writer_Keyvalue_Key keyvalue_key;
 Dirt_Writer_Keyvalue_Value keyvalue_value;
} Dirt_Writer_Callbacks;


typedef char (*Dirt_Writer_Str)(Dirt_Writer *writer, char *str, size_t len);
typedef char (*Dirt_Writer_UnicodeStr)(Dirt_Writer *writer, char *str, size_t len);
typedef char (*Dirt_Writer_Identifier)(Dirt_Writer *writer, char *identifier, size_t len); 
typedef char (*Dirt_Writer_Num_Float)(Dirt_Writer *writer, float real);
typedef char (*Dirt_Writer_Num_Long)(Dirt_Writer *writer, long integer); 
typedef char (*Dirt_Writer_Num_Int)(Dirt_Writer *writer, int integer);
typedef char (*Dirt_Writer_None)(Dirt_Writer *writer);
typedef char (*Dirt_Writer_False)(Dirt_Writer *writer);
typedef char (*Dirt_Writer_True)(Dirt_Writer *writer);
typedef char (*Dirt_Writer_Structure_Tuple)(Dirt_Writer *writer, void *structure);
typedef char (*Dirt_Writer_Structure_List)(Dirt_Writer *writer, void *structure);
typedef char (*Dirt_Writer_Structure_Dictionary)(Dirt_Writer *writer, void *structure);
typedef char (*Dirt_Writer_Structure_Type)(Dirt_Writer *writer, void *structure);
typedef char (*Dirt_Writer_Keyvalue)(Dirt_Writer *writer, void *keyvalue);
typedef char (*Dirt_Writer_Parameter)(Dirt_Writer *writer, void *keyvalue);
typedef char (*Dirt_Writer_Member)(Dirt_Writer *writer, void *keyvalue);
typedef char (*Dirt_Writer_Application)(Dirt_Writer *writer, void *keyvalue);

typedef struct {
 Dirt_Writer_Str                     str;
 Dirt_Writer_UnicodeStr              unicodeStr;
 Dirt_Writer_Identifier              identifier;
 Dirt_Writer_Num_Float               num_float;
 Dirt_Writer_Num_Int                 num_int;
 Dirt_Writer_Num_Long                num_long;
 Dirt_Writer_None                    none;
 Dirt_Writer_False                   false;
 Dirt_Writer_True                    true;
 Dirt_Writer_Structure_Tuple         structure_tuple;
 Dirt_Writer_Structure_List          structure_list;
 Dirt_Writer_Structure_Dictionary    structure_dictionary;
 Dirt_Writer_Structure_Type          structure_type;
 Dirt_Writer_Keyvalue                keyvalue;
 Dirt_Writer_Parameter               parameter;
 Dirt_Writer_Member                  member;
 Dirt_Writer_Application             application;
} Dirt_WriterType;

struct Dirt_WriterT {
 Dirt_WriterType       *type;
 Dirt_Writer_Callbacks *callback;
 Dirt_WriteBuffer      *buffer;
};

char Dirt_StandardWriter_init(Dirt_Writer *writer, Dirt_Writer_Callbacks *callbacks, Dirt_WriteBuffer *buffer);

#endif

