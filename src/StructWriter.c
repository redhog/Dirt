#include <Dirt/StructWriter.h>
#include <Dirt/Struct.h>
#include <stdlib.h>

char Dirt_StructWriter_write(Dirt_Writer *writer, void *any)
 {
  Dirt_Struct *value = (Dirt_Struct *) any;

  if (value->type == &Dirt_StructType_Str)
   return writer->type->str(writer, value->str.str, value->str.len);
  else if (value->type == &Dirt_StructType_UnicodeStr)
   return writer->type->unicodeStr(writer, value->str.str, value->str.len);
  else if (value->type == &Dirt_StructType_Identifier)
   return writer->type->identifier(writer, value->str.str, value->str.len);
  else if (value->type == &Dirt_StructType_Num_Float)
   return writer->type->num_float(writer, value->num_float);
  else if (value->type == &Dirt_StructType_Num_Long)
   return writer->type->num_long(writer, value->num_long);
  else if (value->type == &Dirt_StructType_Num_Int)
   return writer->type->num_int(writer, value->num_int);
  else if (value->type == &Dirt_StructType_None)
   return writer->type->none(writer);
  else if (value->type == &Dirt_StructType_False)
   return writer->type->false(writer);
  else if (value->type == &Dirt_StructType_True)
   return writer->type->true(writer);
  else if (value->type == &Dirt_StructType_Structure_Tuple)
   return writer->type->structure_tuple(writer, (void *) value);
  else if (value->type == &Dirt_StructType_Structure_Tuple)
   return writer->type->structure_tuple(writer, (void *) value);
  else if (value->type == &Dirt_StructType_Structure_List)
   return writer->type->structure_list(writer, (void *) value);
  else if (value->type == &Dirt_StructType_Structure_Dictionary)
   return writer->type->structure_dictionary(writer, (void *) value);
  else if (value->type == &Dirt_StructType_Structure_Type)
   return writer->type->structure_type(writer, (void *) value);
  else if (value->type == &Dirt_StructType_Keyvalue)
   return writer->type->keyvalue(writer, (void *) value);
  else if (value->type == &Dirt_StructType_Parameter)
   return writer->type->parameter(writer, (void *) value);
  else if (value->type == &Dirt_StructType_Member)
   return writer->type->member(writer, (void *) value);
  else if (value->type == &Dirt_StructType_Application)
   return writer->type->application(writer, (void *) value);
  return 0;
 }

typedef struct {
 Dirt_Struct *structure;
 off_t        pos;
} Dirt_StructWriter_Iter;

void *Dirt_StructWriter_structure_open(Dirt_Writer *writer, void *structure)
 {
  Dirt_StructWriter_Iter *iter;

  if (!(iter = (Dirt_StructWriter_Iter *) malloc(sizeof(Dirt_StructWriter_Iter)))) return NULL;
  iter->structure = (Dirt_Struct *) structure;
  iter->pos = 0;
  return (void *) iter;
 }

char  Dirt_StructWriter_structure_end(Dirt_Writer *writer, void *iter)
 {
  Dirt_StructWriter_Iter *i = (Dirt_StructWriter_Iter *) iter;

  return i->pos == i->structure->structure.len;
 }

void *Dirt_StructWriter_structure_next(Dirt_Writer *writer, void *iter)
 {
  Dirt_StructWriter_Iter *i = (Dirt_StructWriter_Iter *) iter;
  void *res;

  res = (void *) i->structure->structure.items[i->pos];
  i->pos++;
  return res;
 }

void Dirt_StructWriter_structure_close(Dirt_Writer *writer, void *iter)
 {
  free(iter);
 }

void *Dirt_StructWriter_keyvalue_key(Dirt_Writer *writer, void *keyvalue)
 {
  return (void *) ((Dirt_Struct *) keyvalue)->keyvalue.key;
 }

void *Dirt_StructWriter_keyvalue_value(Dirt_Writer *writer, void *keyvalue)
 {
  return (void *) ((Dirt_Struct *) keyvalue)->keyvalue.value;
 }


Dirt_Writer_Callbacks Dirt_StructWriter_Callbacks = {
 Dirt_StructWriter_write,
 Dirt_StructWriter_structure_open,
 Dirt_StructWriter_structure_end,
 Dirt_StructWriter_structure_next,
 Dirt_StructWriter_structure_close,
 Dirt_StructWriter_keyvalue_key,
 Dirt_StructWriter_keyvalue_value,
};
