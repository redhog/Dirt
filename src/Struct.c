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
 }

void Dirt_Struct_None_free(Dirt_Session *session, Dirt_Struct *item)
 {
 }

void Dirt_Struct_Structure_free(Dirt_Session *session, Dirt_Struct *item)
 {
  size_t pos;
  for (pos = 0; pos < ((Dirt_StructureStruct *) item)->len; pos++)
   ((Dirt_StructureStruct *) item)->items[pos]->type->decref(session, ((Dirt_StructureStruct *) item)->items[pos]);
 }

void Dirt_Struct_Keyvalue_free(Dirt_Session *session, Dirt_Struct *item)
 {
  ((Dirt_KeyvalueStruct *) item)->key->type->decref(session, ((Dirt_KeyvalueStruct *) item)->key);
  ((Dirt_KeyvalueStruct *) item)->value->type->decref(session, ((Dirt_KeyvalueStruct *) item)->value);
 }

Dirt_Struct *Dirt_Struct_contract(Dirt_Session *session, Dirt_Struct *item)
 {
  return item->type->incref(session, item);
 }

Dirt_Struct *Dirt_StructureStruct_contract(Dirt_Session *session, Dirt_Struct *item)
 {
  Dirt_Struct *contractedItems[((Dirt_StructureStruct *) item)->len];
  char different = 0;
  size_t pos;

  for (pos = 0; pos < ((Dirt_StructureStruct *) item)->len; pos++)
   {
    contractedItems[pos] = ((Dirt_StructureStruct *) item)->items[pos]->type->contract(session, ((Dirt_StructureStruct *) item)->items[pos]);
    if (contractedItems[pos] != ((Dirt_StructureStruct *) item)->items[pos])
     different = 1;
   }
  if (different)
   {
    Dirt_Struct *strct;
    strct = Dirt_Struct_structure(session);
    for (pos = 0; pos < ((Dirt_StructureStruct *) item)->len; pos++)
     strct = Dirt_Struct_structure_add(session, strct, contractedItems[pos]);
    strct->type = item->type;
    return strct;
   }
  else
   {
    for (pos = 0; pos < ((Dirt_StructureStruct *) item)->len; pos++)
     contractedItems[pos]->type->decref(session, contractedItems[pos]);
    return item->type->incref(session, item);
   }
 }

Dirt_Struct *Dirt_KeyvalueStruct_contract(Dirt_Session *session, Dirt_Struct *item)
 {
  Dirt_Struct *key;
  Dirt_Struct *value;

  key = ((Dirt_KeyvalueStruct *) item)->key->type->contract(session, ((Dirt_KeyvalueStruct *) item)->key);
  value = ((Dirt_KeyvalueStruct *) item)->value->type->contract(session, ((Dirt_KeyvalueStruct *) item)->value);

  if (key != ((Dirt_KeyvalueStruct *) item)->key || value != ((Dirt_KeyvalueStruct *) item)->value)
   return Dirt_Struct_keyvalue_init(session, item->type, key, value);
  else
   {
    key->type->decref(session, key);
    value->type->decref(session, value);
    return item->type->incref(session, item);
   }
 }

Dirt_StructType Dirt_StructType_Str = { sizeof(Dirt_StringStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Str_free, Dirt_Struct_contract };
Dirt_StructType Dirt_StructType_UnicodeStr = { sizeof(Dirt_StringStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Str_free, Dirt_Struct_contract };
Dirt_StructType Dirt_StructType_Identifier = { sizeof(Dirt_StringStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Str_free, Dirt_Struct_contract };
Dirt_StructType Dirt_StructType_Num_Float = { sizeof(Dirt_FloatStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_None_free, Dirt_Struct_contract };
Dirt_StructType Dirt_StructType_Num_Long = { sizeof(Dirt_LongStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_None_free, Dirt_Struct_contract };
Dirt_StructType Dirt_StructType_Num_Int = { sizeof(Dirt_IntStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_None_free, Dirt_Struct_contract };
Dirt_StructType Dirt_StructType_None = { sizeof(Dirt_Struct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_None_free, Dirt_Struct_contract };
Dirt_StructType Dirt_StructType_False = { sizeof(Dirt_Struct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_None_free, Dirt_Struct_contract };
Dirt_StructType Dirt_StructType_True = { sizeof(Dirt_Struct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_None_free, Dirt_Struct_contract };
Dirt_StructType Dirt_StructType_Structure = { sizeof(Dirt_StructureStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Structure_free, Dirt_StructureStruct_contract };
Dirt_StructType Dirt_StructType_Structure_Tuple = { sizeof(Dirt_StructureStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Structure_free, Dirt_StructureStruct_contract };
Dirt_StructType Dirt_StructType_Structure_List = { sizeof(Dirt_StructureStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Structure_free, Dirt_StructureStruct_contract };
Dirt_StructType Dirt_StructType_Structure_Dictionary = { sizeof(Dirt_StructureStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Structure_free, Dirt_StructureStruct_contract };
Dirt_StructType Dirt_StructType_Structure_Type = { sizeof(Dirt_StructureStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Structure_free, Dirt_StructureStruct_contract };
Dirt_StructType Dirt_StructType_Keyvalue = { sizeof(Dirt_KeyvalueStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Keyvalue_free, Dirt_KeyvalueStruct_contract };
Dirt_StructType Dirt_StructType_Parameter = { sizeof(Dirt_KeyvalueStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Keyvalue_free, Dirt_KeyvalueStruct_contract };
Dirt_StructType Dirt_StructType_Member = { sizeof(Dirt_KeyvalueStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Keyvalue_free, Dirt_KeyvalueStruct_contract };
Dirt_StructType Dirt_StructType_Application = { sizeof(Dirt_KeyvalueStruct), Dirt_Struct_decref, Dirt_Struct_incref, Dirt_Struct_Keyvalue_free, Dirt_KeyvalueStruct_contract };


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
 }

Dirt_Struct *Dirt_Struct_str(Dirt_Session *session, char *str, size_t len) { return Dirt_Struct_init_str(session, &Dirt_StructType_Str, str, len); }
Dirt_Struct *Dirt_Struct_unicodeStr(Dirt_Session *session, char *str, size_t len) { return Dirt_Struct_init_str(session, &Dirt_StructType_UnicodeStr, str, len); }
Dirt_Struct *Dirt_Struct_identifier(Dirt_Session *session, char *str, size_t len) { return Dirt_Struct_init_str(session, &Dirt_StructType_Identifier, str, len); }

Dirt_Struct *Dirt_Struct_num_float(Dirt_Session *session, float real)
 {
  Dirt_Struct *strct;
  if (!(strct = Dirt_Struct_init(session, &Dirt_StructType_Num_Float))) return NULL;
  ((Dirt_FloatStruct *) strct)->num_float = real;
  return strct;
 }

Dirt_Struct *Dirt_Struct_num_long(Dirt_Session *session, long integer)
 {
  Dirt_Struct *strct;
  if (!(strct = Dirt_Struct_init(session, &Dirt_StructType_Num_Long))) return NULL;
  ((Dirt_LongStruct *) strct)->num_long = integer;
  return strct;
 }

Dirt_Struct *Dirt_Struct_num_int(Dirt_Session *session, int integer)
 {
  Dirt_Struct *strct;
  if (!(strct = Dirt_Struct_init(session, &Dirt_StructType_Num_Int))) return NULL;
  ((Dirt_IntStruct *) strct)->num_int = integer;
  return strct;
 }

Dirt_Struct *Dirt_Struct_none(Dirt_Session *session) { return Dirt_Struct_init(session, &Dirt_StructType_None); }
Dirt_Struct *Dirt_Struct_false(Dirt_Session *session) { return Dirt_Struct_init(session, &Dirt_StructType_False); }
Dirt_Struct *Dirt_Struct_true(Dirt_Session *session) { return Dirt_Struct_init(session, &Dirt_StructType_True); }

Dirt_Struct *Dirt_Struct_structure(Dirt_Session *session)
 {
  Dirt_Struct *strct;
  if (!(strct = Dirt_Struct_init(session, &Dirt_StructType_Structure))) return NULL;
  ((Dirt_StructureStruct *) strct)->items = NULL;
  ((Dirt_StructureStruct *) strct)->len = 0;
  return strct;
 }

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
 }

Dirt_Struct *Dirt_Struct_keyvalue(Dirt_Session *session, Dirt_Struct *key, Dirt_Struct *value) { return Dirt_Struct_keyvalue_init(session, &Dirt_StructType_Keyvalue, key, value); }
Dirt_Struct *Dirt_Struct_parameter(Dirt_Session *session, Dirt_Struct *name, Dirt_Struct *value) { return Dirt_Struct_keyvalue_init(session, &Dirt_StructType_Parameter, name, value); }
Dirt_Struct *Dirt_Struct_member(Dirt_Session *session, Dirt_Struct *name, Dirt_Struct *value) { return Dirt_Struct_keyvalue_init(session, &Dirt_StructType_Member, name, value); }
Dirt_Struct *Dirt_Struct_application(Dirt_Session *session, Dirt_Struct *function, Dirt_Struct *parameters) { return Dirt_Struct_keyvalue_init(session, &Dirt_StructType_Application, function, parameters); }

Dirt_Struct *Dirt_Struct_structure_add_array(Dirt_Session *session, Dirt_Struct *structure, Dirt_Struct **item, size_t items)
 {
  Dirt_StructureStruct *strct;
  Dirt_Struct **strctitems;

  if (!structure || !item)
   {
    if (structure) structure->type->decref(session, structure);
    if (item)
     for (;items;items--,item++)
      (*item)->type->decref(session, *item);
    return NULL;
   }
  strct = (Dirt_StructureStruct *) structure;
  if (!(strctitems = realloc(strct->items, sizeof(Dirt_Struct *) * (strct->len + items))))
   {
    structure->type->decref(session, structure);
    session->type->error(session, "Memory", "Out of memory when adding items to structure");
    return NULL;
   }
  strct->items = strctitems;
  memcpy((void *) (strct->items + strct->len), (void *) item, sizeof(Dirt_Struct *) * items);
  strct->len += items;
  return structure;
 }

Dirt_Struct *Dirt_Struct_structure_add_pairs(Dirt_Session *session, Dirt_Struct *structure, Dirt_StructType *type, char **name, Dirt_Struct **item, size_t items)
 {
  Dirt_StructureStruct *strct;
  Dirt_Struct **strctitems;

  if (!structure || !name || !item)
   {
    if (structure) structure->type->decref(session, structure);
    if (item)
     for (;items;items--,item++)
      (*item)->type->decref(session, *item);
    return NULL;
   }
  strct = (Dirt_StructureStruct *) structure;
  if (!(strctitems = realloc(strct->items, sizeof(Dirt_Struct *) * items)))
   {
    structure->type->decref(session, structure);
    session->type->error(session, "Memory", "Out of memory when adding items to structure");
    return NULL;
   }
  strct->items = strctitems;
  for (strct->len = 0; strct->len < items; strct->len++)
   if (!(strct->items[strct->len] = Dirt_Struct_keyvalue_init(session, type,
							      Dirt_Struct_unicodeStr(session,
										     name[strct->len],
										     strlen(name[strct->len])),
							      item[strct->len])))
    {
     structure->type->decref(session, structure);
     return NULL;
    }

  return structure;
 }

char Dirt_Struct_structure_to_array(Dirt_Session *session, Dirt_Struct *structure, Dirt_Struct ***item, size_t *items)
 {
  Dirt_StructureStruct *strct = (Dirt_StructureStruct *) structure;
  size_t pos;

  if (!structure || !item || !items)
   {
    if (structure) structure->type->decref(session, structure);
    return 0;
   }
  if (!(*item = malloc(sizeof(Dirt_Struct *) * strct->len)))
   {
    structure->type->decref(session, structure);
    return 0;
   }
  *items = strct->len;
  for (pos = 0; pos < strct->len; pos++)
   if (!((*item)[pos] = strct->items[pos]->type->incref(session, strct->items[pos])))
    {
     structure->type->decref(session, structure);
     for (; pos > 0; pos--)
      (*item)[pos-1]->type->decref(session, (*item)[pos-1]);
     return 0;
    }
  return 1;
 }

char Dirt_Struct_structure_to_pairs(Dirt_Session *session, Dirt_Struct *structure, Dirt_StructType **type, char ***name, Dirt_Struct ***item, size_t *items)
 {
  Dirt_StructureStruct *strct = (Dirt_StructureStruct *) structure;
  Dirt_KeyvalueStruct *pair;
  Dirt_StringStruct *key;
  size_t pos;

  if (!structure || !item || !items)
   {
    if (structure) structure->type->decref(session, structure);
    return 0;
   }
  if (!(*item = malloc(sizeof(Dirt_Struct *) * strct->len)))
   {
    structure->type->decref(session, structure);
    return 0;
   }
  if (!(*name = malloc(sizeof(char *) * strct->len)))
   {
    free(*item);
    *item = NULL;
    structure->type->decref(session, structure);
    return 0;
   }
  *items = strct->len;
  *type = NULL;
  for (pos = 0; pos < strct->len; pos++)
   {
    if (!*type)
     {
      *type = strct->items[pos]->type;
      if (   *type != &Dirt_StructType_Keyvalue
	  && *type != &Dirt_StructType_Parameter
	  && *type != &Dirt_StructType_Member
	  && *type != &Dirt_StructType_Application)
       goto Dirt_Struct_structure_to_pairs_failed;
     }
    pair = (Dirt_KeyvalueStruct *) strct->items[pos];
    if (strct->items[pos]->type != *type
        || (   pair->key->type != &Dirt_StructType_UnicodeStr
	    && pair->key->type != &Dirt_StructType_Str))
     goto Dirt_Struct_structure_to_pairs_failed;
    key = (Dirt_StringStruct *) pair->key;
    if (!((*name)[pos] = malloc(key->len + 1)))
     goto Dirt_Struct_structure_to_pairs_failed;
    memcpy((*name)[pos], key->str, key->len);
    (*name)[pos][key->len] = '\0';
    if (!((*item)[pos] = pair->value->type->incref(session, pair->value)))
     {
      free((*name)[pos]);
      goto Dirt_Struct_structure_to_pairs_failed;
     }

   }
  return 1;
 Dirt_Struct_structure_to_pairs_failed:
  structure->type->decref(session, structure);
  for (; pos > 0; pos--)
   {
    free((*name)[pos-1]);
    (*item)[pos-1]->type->decref(session, (*item)[pos-1]);
   }
  return 0;
 }
