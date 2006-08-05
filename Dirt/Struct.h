#ifndef DIRT_STRUCT_H
#define DIRT_STRUCT_H

#include <Dirt/Session.h>
#include <string.h>

typedef struct Dirt_StructT Dirt_Struct;

typedef void (*Dirt_Struct_Free)(Dirt_Session *session, Dirt_Struct *item);

typedef struct {
 Dirt_Struct_Free free;
 size_t size;
} Dirt_StructType;

struct Dirt_StructT {
 Dirt_StructType *type;
};

typedef struct {
 Dirt_Struct base;
 char *str;
 size_t len;
} Dirt_StringStruct;

typedef struct {
 Dirt_Struct base;
 float num_float;
} Dirt_FloatStruct;

typedef struct {
 Dirt_Struct base;
 long num_long;
} Dirt_LongStruct;

typedef struct {
 Dirt_Struct base;
 int num_int;
} Dirt_IntStruct;

typedef struct {
 Dirt_Struct base;
 Dirt_Struct **items;
 size_t len;
} Dirt_StructureStruct;

typedef struct {
 Dirt_Struct base;
 Dirt_Struct *key;
 Dirt_Struct *value;
} Dirt_KeyvalueStruct;

Dirt_StructType Dirt_StructType_Str;
Dirt_StructType Dirt_StructType_UnicodeStr;
Dirt_StructType Dirt_StructType_Identifier;
Dirt_StructType Dirt_StructType_Num_Float;
Dirt_StructType Dirt_StructType_Num_Long;
Dirt_StructType Dirt_StructType_Num_Int;
Dirt_StructType Dirt_StructType_None;
Dirt_StructType Dirt_StructType_False;
Dirt_StructType Dirt_StructType_True;
Dirt_StructType Dirt_StructType_Structure;
Dirt_StructType Dirt_StructType_Structure_Tuple;
Dirt_StructType Dirt_StructType_Structure_List;
Dirt_StructType Dirt_StructType_Structure_Dictionary;
Dirt_StructType Dirt_StructType_Structure_Type;
Dirt_StructType Dirt_StructType_Keyvalue;
Dirt_StructType Dirt_StructType_Parameter;
Dirt_StructType Dirt_StructType_Member;
Dirt_StructType Dirt_StructType_Application;

Dirt_Struct *Dirt_Struct_str(Dirt_Session *session, char *str, size_t len);
Dirt_Struct *Dirt_Struct_unicodeStr(Dirt_Session *session, char *str, size_t len);
Dirt_Struct *Dirt_Struct_identifier(Dirt_Session *session, char *str, size_t len);
Dirt_Struct *Dirt_Struct_num_float(Dirt_Session *session, float real);
Dirt_Struct *Dirt_Struct_num_long(Dirt_Session *session, long integer);
Dirt_Struct *Dirt_Struct_num_int(Dirt_Session *session, int integer);
Dirt_Struct *Dirt_Struct_none(Dirt_Session *session);
Dirt_Struct *Dirt_Struct_false(Dirt_Session *session);
Dirt_Struct *Dirt_Struct_true(Dirt_Session *session);
Dirt_Struct *Dirt_Struct_structure(Dirt_Session *session);
Dirt_Struct *Dirt_Struct_structure_add(Dirt_Session *session, Dirt_Struct *structure, Dirt_Struct *item);
Dirt_Struct *Dirt_Struct_structure_finalize_tuple(Dirt_Session *session, Dirt_Struct *structure);
Dirt_Struct *Dirt_Struct_structure_finalize_list(Dirt_Session *session, Dirt_Struct *structure);
Dirt_Struct *Dirt_Struct_structure_finalize_dictionary(Dirt_Session *session, Dirt_Struct *structure);
Dirt_Struct *Dirt_Struct_structure_finalize_type(Dirt_Session *session, Dirt_Struct *structure);
Dirt_Struct *Dirt_Struct_keyvalue(Dirt_Session *session, Dirt_Struct *key, Dirt_Struct *value);
Dirt_Struct *Dirt_Struct_parameter(Dirt_Session *session, Dirt_Struct *name, Dirt_Struct *value);
Dirt_Struct *Dirt_Struct_member(Dirt_Session *session, Dirt_Struct *name, Dirt_Struct *value);
Dirt_Struct *Dirt_Struct_application(Dirt_Session *session, Dirt_Struct *function, Dirt_Struct *parameters);

#endif
