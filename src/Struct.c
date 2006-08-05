#include <Dirt/StructImplementor.h>
#include <string.h>
#include <stdlib.h>

void Dirt_Struct_Str_free(Dirt_Session *session, Dirt_Struct *item)
 {
  free(((Dirt_StringStruct *) item)->str);
 };

void Dirt_Struct_None_free(Dirt_Session *session, Dirt_Struct *item)
 {
 };

void Dirt_Struct_Structure_free(Dirt_Session *session, Dirt_Struct *item)
 {
  size_t pos;
  for (pos = 0; pos < ((Dirt_StructureStruct *) item)->len; pos++)
   ((Dirt_StructureStruct *) item)->items[pos]->type->free(session, ((Dirt_StructureStruct *) item)->items[pos]);
 };

void Dirt_Struct_Keyvalue_free(Dirt_Session *session, Dirt_Struct *item)
 {
  ((Dirt_KeyvalueStruct *) item)->key->type->free(session, ((Dirt_KeyvalueStruct *) item)->key);
  ((Dirt_KeyvalueStruct *) item)->value->type->free(session, ((Dirt_KeyvalueStruct *) item)->value);
 };

Dirt_StructType Dirt_StructType_Str = { Dirt_Struct_Str_free, sizeof(Dirt_StringStruct) };
Dirt_StructType Dirt_StructType_UnicodeStr = { Dirt_Struct_Str_free, sizeof(Dirt_StringStruct) };
Dirt_StructType Dirt_StructType_Identifier = { Dirt_Struct_Str_free, sizeof(Dirt_StringStruct) };
Dirt_StructType Dirt_StructType_Num_Float = { Dirt_Struct_None_free, sizeof(Dirt_FloatStruct) };
Dirt_StructType Dirt_StructType_Num_Long = { Dirt_Struct_None_free, sizeof(Dirt_LongStruct) };
Dirt_StructType Dirt_StructType_Num_Int = { Dirt_Struct_None_free, sizeof(Dirt_IntStruct) };
Dirt_StructType Dirt_StructType_None = { Dirt_Struct_None_free, sizeof(Dirt_Struct) };
Dirt_StructType Dirt_StructType_False = { Dirt_Struct_None_free, sizeof(Dirt_Struct) };
Dirt_StructType Dirt_StructType_True = { Dirt_Struct_None_free, sizeof(Dirt_Struct) };
Dirt_StructType Dirt_StructType_Structure = { Dirt_Struct_Structure_free, sizeof(Dirt_StructureStruct) };
Dirt_StructType Dirt_StructType_Structure_Tuple = { Dirt_Struct_Structure_free, sizeof(Dirt_StructureStruct) };
Dirt_StructType Dirt_StructType_Structure_List = { Dirt_Struct_Structure_free, sizeof(Dirt_StructureStruct) };
Dirt_StructType Dirt_StructType_Structure_Dictionary = { Dirt_Struct_Structure_free, sizeof(Dirt_StructureStruct) };
Dirt_StructType Dirt_StructType_Structure_Type = { Dirt_Struct_Structure_free, sizeof(Dirt_StructureStruct) };
Dirt_StructType Dirt_StructType_Keyvalue = { Dirt_Struct_Keyvalue_free, sizeof(Dirt_KeyvalueStruct) };
Dirt_StructType Dirt_StructType_Parameter = { Dirt_Struct_Keyvalue_free, sizeof(Dirt_KeyvalueStruct) };
Dirt_StructType Dirt_StructType_Member = { Dirt_Struct_Keyvalue_free, sizeof(Dirt_KeyvalueStruct) };
Dirt_StructType Dirt_StructType_Application = { Dirt_Struct_Keyvalue_free, sizeof(Dirt_KeyvalueStruct) };


Dirt_Struct *Dirt_Struct_init(Dirt_Session *session, Dirt_StructType *type)
 {
  Dirt_Struct *strct;
  if (strct = malloc(type->size))
   strct->type = type;
  return strct;
 }

Dirt_Struct *Dirt_Struct_init_str(Dirt_Session *session, Dirt_StructType *type, char *str, size_t len)
 {
  Dirt_Struct *strct; if (!(strct = Dirt_Struct_init(session, type))) return NULL;
  ((Dirt_StringStruct *) strct)->str = malloc(len);
  memcpy(((Dirt_StringStruct *) strct)->str, str, len);
  ((Dirt_StringStruct *) strct)->len = len;
  return strct;
 };

Dirt_Struct *Dirt_Struct_str(Dirt_Session *session, char *str, size_t len) { return Dirt_Struct_init_str(session, &Dirt_StructType_Str, str, len); };
Dirt_Struct *Dirt_Struct_unicodeStr(Dirt_Session *session, char *str, size_t len) { return Dirt_Struct_init_str(session, &Dirt_StructType_UnicodeStr, str, len); };
Dirt_Struct *Dirt_Struct_identifier(Dirt_Session *session, char *str, size_t len) { return Dirt_Struct_init_str(session, &Dirt_StructType_Identifier, str, len); };

Dirt_Struct *Dirt_Struct_num_float(Dirt_Session *session, float real)
 {
  Dirt_Struct *strct;
  if (!(strct = Dirt_Struct_init(session, &Dirt_StructType_Num_Float))) return NULL;
  ((Dirt_FloatStruct *) strct)->num_float = real;
  return strct;
 };

Dirt_Struct *Dirt_Struct_num_long(Dirt_Session *session, long integer)
 {
  Dirt_Struct *strct;
  if (!(strct = Dirt_Struct_init(session, &Dirt_StructType_Num_Long))) return NULL;
  ((Dirt_LongStruct *) strct)->num_long = integer;
  return strct;
 };

Dirt_Struct *Dirt_Struct_num_int(Dirt_Session *session, int integer)
 {
  Dirt_Struct *strct;
  if (!(strct = Dirt_Struct_init(session, &Dirt_StructType_Num_Int))) return NULL;
  ((Dirt_IntStruct *) strct)->num_int = integer;
  return strct;
 };

Dirt_Struct *Dirt_Struct_none(Dirt_Session *session) { return Dirt_Struct_init(session, &Dirt_StructType_None); };
Dirt_Struct *Dirt_Struct_false(Dirt_Session *session) { return Dirt_Struct_init(session, &Dirt_StructType_False); };
Dirt_Struct *Dirt_Struct_true(Dirt_Session *session) { return Dirt_Struct_init(session, &Dirt_StructType_True); };

Dirt_Struct *Dirt_Struct_structure(Dirt_Session *session)
 {
  Dirt_Struct *strct;
  if (!(strct = Dirt_Struct_init(session, &Dirt_StructType_Structure))) return NULL;
  ((Dirt_StructureStruct *) strct)->items = NULL;
  ((Dirt_StructureStruct *) strct)->len = 0;
  return strct;
 };

Dirt_Struct *Dirt_Struct_structure_add(Dirt_Session *session, Dirt_Struct *structure, Dirt_Struct *item)
 {
  Dirt_StructureStruct *strct = (Dirt_StructureStruct *) structure;
  Dirt_Struct **items;
  if (!(items = realloc(strct->items, sizeof(Dirt_Struct *) * (strct->len + 1))))
   {
    structure->type->free(session, structure);
    session->type->error(session, "Memory", "Out of memory when adding item at end of structure");
    return NULL;
   }
  strct->items = items;
  strct->items[strct->len++] = (Dirt_Struct *) item;
  return structure;
 }

Dirt_Struct *Dirt_Struct_structure_finalize_tuple(Dirt_Session *session, Dirt_Struct *structure) { structure->type = &Dirt_StructType_Structure_Tuple; return structure; }
Dirt_Struct *Dirt_Struct_structure_finalize_list(Dirt_Session *session, Dirt_Struct *structure) { structure->type = &Dirt_StructType_Structure_List; return structure; }
Dirt_Struct *Dirt_Struct_structure_finalize_dictionary(Dirt_Session *session, Dirt_Struct *structure) { structure->type = &Dirt_StructType_Structure_Dictionary; return structure; }
Dirt_Struct *Dirt_Struct_structure_finalize_type(Dirt_Session *session, Dirt_Struct *structure) { structure->type = &Dirt_StructType_Structure_Type; return structure; }

Dirt_Struct *Dirt_Struct_keyvalue_init(Dirt_Session *session, Dirt_StructType *type, Dirt_Struct *key, Dirt_Struct *value)
 {
  Dirt_Struct *strct; if (!(strct = Dirt_Struct_init(session, type))) return NULL;
  ((Dirt_KeyvalueStruct *) strct)->key = key;
  ((Dirt_KeyvalueStruct *) strct)->value = value;
  return strct;
 };

Dirt_Struct *Dirt_Struct_keyvalue(Dirt_Session *session, Dirt_Struct *key, Dirt_Struct *value) { return Dirt_Struct_keyvalue_init(session, &Dirt_StructType_Keyvalue, key, value); }
Dirt_Struct *Dirt_Struct_parameter(Dirt_Session *session, Dirt_Struct *name, Dirt_Struct *value) { return Dirt_Struct_keyvalue_init(session, &Dirt_StructType_Parameter, name, value); }
Dirt_Struct *Dirt_Struct_member(Dirt_Session *session, Dirt_Struct *name, Dirt_Struct *value) { return Dirt_Struct_keyvalue_init(session, &Dirt_StructType_Member, name, value); }
Dirt_Struct *Dirt_Struct_application(Dirt_Session *session, Dirt_Struct *function, Dirt_Struct *parameters) { return Dirt_Struct_keyvalue_init(session, &Dirt_StructType_Application, function, parameters); }
