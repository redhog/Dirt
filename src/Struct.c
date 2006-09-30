#include <Dirt/StructImplementor.h>
#include <string.h>
#include <stdlib.h>

Dirt_Struct *Dirt_Struct_init_str(Dirt_Session *session, Dirt_StructType *type, char *str, size_t len);
Dirt_Struct *Dirt_Struct_keyvalue_init(Dirt_Session *session, Dirt_StructType *type, Dirt_Struct *key, Dirt_Struct *value);

void Dirt_Struct_decref(Dirt_Session *session, Dirt_Struct *item)
 {
  if (!--item->references)
   item->type->free(session, item);
 }

Dirt_Struct *Dirt_Struct_incref(Dirt_Session *session, Dirt_Struct *item)
 {
  item->references++;
  return item;
 }

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
   ((Dirt_StructureStruct *) item)->items[pos]->type->decref(session, ((Dirt_StructureStruct *) item)->items[pos]);
 };

void Dirt_Struct_Keyvalue_free(Dirt_Session *session, Dirt_Struct *item)
 {
  ((Dirt_KeyvalueStruct *) item)->key->type->decref(session, ((Dirt_KeyvalueStruct *) item)->key);
  ((Dirt_KeyvalueStruct *) item)->value->type->decref(session, ((Dirt_KeyvalueStruct *) item)->value);
 };

Dirt_Struct *Dirt_Struct_restrict(Dirt_Session *session, Dirt_Struct *item)
 {
  return item->type->incref(session, item);
 }

Dirt_Struct *Dirt_StructureStruct_restrict(Dirt_Session *session, Dirt_Struct *item)
 {
  Dirt_Struct *restrictedItems[((Dirt_StructureStruct *) item)->len];
  char different = 0;
  size_t pos;

  for (pos = 0; pos < ((Dirt_StructureStruct *) item)->len; pos++)
   {
    restrictedItems[pos] = ((Dirt_StructureStruct *) item)->items[pos]->type->restrict(session, ((Dirt_StructureStruct *) item)->items[pos]);
    if (restrictedItems[pos] != ((Dirt_StructureStruct *) item)->items[pos])
     different = 1;
   }
  if (different)
   {
    Dirt_Struct *strct;
    strct = Dirt_Struct_structure(session);
    for (pos = 0; pos < ((Dirt_StructureStruct *) item)->len; pos++)
     strct = Dirt_Struct_structure_add(session, strct, restrictedItems[pos]);
    strct->type = item->type;
    return strct;
   }
  else
   {
    for (pos = 0; pos < ((Dirt_StructureStruct *) item)->len; pos++)
     restrictedItems[pos]->type->decref(session, restrictedItems[pos]);
    return item->type->incref(session, item);
   }
 }

Dirt_Struct *Dirt_KeyvalueStruct_restrict(Dirt_Session *session, Dirt_Struct *item)
 {
  Dirt_Struct *key;
  Dirt_Struct *value;

  key = ((Dirt_KeyvalueStruct *) item)->key->type->restrict(session, ((Dirt_KeyvalueStruct *) item)->key);
  value = ((Dirt_KeyvalueStruct *) item)->value->type->restrict(session, ((Dirt_KeyvalueStruct *) item)->value);

  if (key != ((Dirt_KeyvalueStruct *) item)->key || value != ((Dirt_KeyvalueStruct *) item)->value)
   return Dirt_Struct_keyvalue_init(session, item->type, key, value);
  else
   {
    key->type->decref(session, key);
    value->type->decref(session, value);
    return item->type->incref(session, item);
   }
 }

Dirt_StructType Dirt_StructType_Str = { sizeof(Dirt_StringStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Str_free, Dirt_Struct_restrict };
Dirt_StructType Dirt_StructType_UnicodeStr = { sizeof(Dirt_StringStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Str_free, Dirt_Struct_restrict };
Dirt_StructType Dirt_StructType_Identifier = { sizeof(Dirt_StringStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Str_free, Dirt_Struct_restrict };
Dirt_StructType Dirt_StructType_Num_Float = { sizeof(Dirt_FloatStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_None_free, Dirt_Struct_restrict };
Dirt_StructType Dirt_StructType_Num_Long = { sizeof(Dirt_LongStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_None_free, Dirt_Struct_restrict };
Dirt_StructType Dirt_StructType_Num_Int = { sizeof(Dirt_IntStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_None_free, Dirt_Struct_restrict };
Dirt_StructType Dirt_StructType_None = { sizeof(Dirt_Struct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_None_free, Dirt_Struct_restrict };
Dirt_StructType Dirt_StructType_False = { sizeof(Dirt_Struct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_None_free, Dirt_Struct_restrict };
Dirt_StructType Dirt_StructType_True = { sizeof(Dirt_Struct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_None_free, Dirt_Struct_restrict };
Dirt_StructType Dirt_StructType_Structure = { sizeof(Dirt_StructureStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Structure_free, Dirt_StructureStruct_restrict };
Dirt_StructType Dirt_StructType_Structure_Tuple = { sizeof(Dirt_StructureStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Structure_free, Dirt_StructureStruct_restrict };
Dirt_StructType Dirt_StructType_Structure_List = { sizeof(Dirt_StructureStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Structure_free, Dirt_StructureStruct_restrict };
Dirt_StructType Dirt_StructType_Structure_Dictionary = { sizeof(Dirt_StructureStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Structure_free, Dirt_StructureStruct_restrict };
Dirt_StructType Dirt_StructType_Structure_Type = { sizeof(Dirt_StructureStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Structure_free, Dirt_StructureStruct_restrict };
Dirt_StructType Dirt_StructType_Keyvalue = { sizeof(Dirt_KeyvalueStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Keyvalue_free, Dirt_KeyvalueStruct_restrict };
Dirt_StructType Dirt_StructType_Parameter = { sizeof(Dirt_KeyvalueStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Keyvalue_free, Dirt_KeyvalueStruct_restrict };
Dirt_StructType Dirt_StructType_Member = { sizeof(Dirt_KeyvalueStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Keyvalue_free, Dirt_KeyvalueStruct_restrict };
Dirt_StructType Dirt_StructType_Application = { sizeof(Dirt_KeyvalueStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Keyvalue_free, Dirt_KeyvalueStruct_restrict };


Dirt_Struct *Dirt_Struct_init(Dirt_Session *session, Dirt_StructType *type)
 {
  Dirt_Struct *strct;
  if (!(strct = malloc(type->size))) return NULL;
  strct->type = type;
  strct->references = 1;
  return strct;
 }

Dirt_Struct *Dirt_Struct_init_str(Dirt_Session *session, Dirt_StructType *type, char *str, size_t len)
 {
  Dirt_Struct *strct;
  
  if (!str) return NULL;
  if (!(strct = Dirt_Struct_init(session, type))) return NULL;
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
  if (!structure || !item)
   {
    if (structure) structure->type->decref(session, structure);
    if (item) item->type->decref(session, item);
    return NULL;
   }
  if (!(items = realloc(strct->items, sizeof(Dirt_Struct *) * (strct->len + 1))))
   {
    structure->type->decref(session, structure);
    session->type->error(session, "Memory", "Out of memory when adding item at end of structure");
    return NULL;
   }
  strct->items = items;
  strct->items[strct->len++] = (Dirt_Struct *) item;
  return structure;
 }

Dirt_Struct *Dirt_Struct_structure_finalize_tuple(Dirt_Session *session, Dirt_Struct *structure) { if (structure) structure->type = &Dirt_StructType_Structure_Tuple; return structure; }
Dirt_Struct *Dirt_Struct_structure_finalize_list(Dirt_Session *session, Dirt_Struct *structure) { if (structure) structure->type = &Dirt_StructType_Structure_List; return structure; }
Dirt_Struct *Dirt_Struct_structure_finalize_dictionary(Dirt_Session *session, Dirt_Struct *structure) { if (structure) structure->type = &Dirt_StructType_Structure_Dictionary; return structure; }
Dirt_Struct *Dirt_Struct_structure_finalize_type(Dirt_Session *session, Dirt_Struct *structure) { if (structure) structure->type = &Dirt_StructType_Structure_Type; return structure; }

Dirt_Struct *Dirt_Struct_keyvalue_init(Dirt_Session *session, Dirt_StructType *type, Dirt_Struct *key, Dirt_Struct *value)
 {
  Dirt_Struct *strct;
  if (!key || !value)
   {
    if (key) key->type->decref(session, key);
    if (value) value->type->decref(session, value);
    return NULL;
   }
  if (!(strct = Dirt_Struct_init(session, type))) return NULL;
  ((Dirt_KeyvalueStruct *) strct)->key = key;
  ((Dirt_KeyvalueStruct *) strct)->value = value;
  return strct;
 };

Dirt_Struct *Dirt_Struct_keyvalue(Dirt_Session *session, Dirt_Struct *key, Dirt_Struct *value) { return Dirt_Struct_keyvalue_init(session, &Dirt_StructType_Keyvalue, key, value); }
Dirt_Struct *Dirt_Struct_parameter(Dirt_Session *session, Dirt_Struct *name, Dirt_Struct *value) { return Dirt_Struct_keyvalue_init(session, &Dirt_StructType_Parameter, name, value); }
Dirt_Struct *Dirt_Struct_member(Dirt_Session *session, Dirt_Struct *name, Dirt_Struct *value) { return Dirt_Struct_keyvalue_init(session, &Dirt_StructType_Member, name, value); }
Dirt_Struct *Dirt_Struct_application(Dirt_Session *session, Dirt_Struct *function, Dirt_Struct *parameters) { return Dirt_Struct_keyvalue_init(session, &Dirt_StructType_Application, function, parameters); }

Dirt_Struct *Dirt_Struct_structure_from_array(Dirt_Session *session, Dirt_Struct **item, size_t items)
 {
  size_t pos;
  Dirt_Struct *res;

  res = Dirt_Struct_structure(session);
  for (pos = 0; pos < items; pos++)
   res = Dirt_Struct_structure_add(session, res, item[pos]);
  return res;
 }

char Dirt_Struct_structure_to_array(Dirt_Session *session, Dirt_Struct *structure, Dirt_Struct ***item, size_t *items)
 {
  Dirt_StructureStruct *strct = (Dirt_StructureStruct *) structure;
  ssize_t pos;

  if (!structure || !item || !items) return 0;
  if (!(*item = malloc(sizeof(Dirt_Struct *) * strct->len))) return 0;
  *items = strct->len;
  for (pos = 0; pos < strct->len; pos++)
   if (!((*item)[pos] = strct->items[pos]->type->incref(session, strct->items[pos])))
    {
     pos--;
     for (; pos >= 0; pos--)
      strct->items[pos]->type->decref(session, strct->items[pos]);
     return 0;
    }
  return 1;
 }
