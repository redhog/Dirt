#include <Dirt/StructReader.h>
#include <Dirt/Struct.h>
#include <string.h>

void  Dirt_StructReader_free(Dirt_Reader *reader, void *item) { ((Dirt_Struct *) item)->type->free(reader->buffer->session, (Dirt_Struct *) item); };
void *Dirt_StructReader_str(Dirt_Reader *reader, char *str, size_t len) { return (void *) Dirt_Struct_str(reader->buffer->session, str, len); };
void *Dirt_StructReader_unicodeStr(Dirt_Reader *reader, char *str, size_t len) { return (void *) Dirt_Struct_unicodeStr(reader->buffer->session, str, len); };
void *Dirt_StructReader_identifier(Dirt_Reader *reader, char *str, size_t len) { return (void *) Dirt_Struct_identifier(reader->buffer->session, str, len); };
void *Dirt_StructReader_num_float(Dirt_Reader *reader, float real) { return (void *) Dirt_Struct_num_float(reader->buffer->session, real); };
void *Dirt_StructReader_num_long(Dirt_Reader *reader, long integer) { return (void *) Dirt_Struct_num_long(reader->buffer->session, integer); };
void *Dirt_StructReader_num_int(Dirt_Reader *reader, int integer) { return (void *) Dirt_Struct_num_int(reader->buffer->session, integer); };
void *Dirt_StructReader_none(Dirt_Reader *reader) { return (void *) Dirt_Struct_none(reader->buffer->session); };
void *Dirt_StructReader_false(Dirt_Reader *reader) { return (void *) Dirt_Struct_false(reader->buffer->session); };
void *Dirt_StructReader_true(Dirt_Reader *reader) { return (void *) Dirt_Struct_true(reader->buffer->session); };
void *Dirt_StructReader_structure(Dirt_Reader *reader) { return (void *) Dirt_Struct_structure(reader->buffer->session); };
void *Dirt_StructReader_structure_add(Dirt_Reader *reader, void *structure, void *item)
 {
  return (void *) Dirt_Struct_structure_add(reader->buffer->session, (Dirt_Struct *) structure, (Dirt_Struct *) item);
 };
void *Dirt_StructReader_structure_finalize_tuple(Dirt_Reader *reader, void *structure) { return (void *) Dirt_Struct_structure_finalize_tuple(reader->buffer->session, (Dirt_Struct *) structure); };
void *Dirt_StructReader_structure_finalize_list(Dirt_Reader *reader, void *structure) { return (void *) Dirt_Struct_structure_finalize_list(reader->buffer->session, (Dirt_Struct *) structure); };
void *Dirt_StructReader_structure_finalize_dictionary(Dirt_Reader *reader, void *structure) { return (void *) Dirt_Struct_structure_finalize_dictionary(reader->buffer->session, (Dirt_Struct *) structure); };
void *Dirt_StructReader_structure_finalize_type(Dirt_Reader *reader, void *structure) { return (void *) Dirt_Struct_structure_finalize_type(reader->buffer->session, (Dirt_Struct *) structure); };
void *Dirt_StructReader_keyvalue(Dirt_Reader *reader, void *key, void *value) { return (void *) Dirt_Struct_keyvalue(reader->buffer->session, (Dirt_Struct *) key, (Dirt_Struct *) value); };
void *Dirt_StructReader_parameter(Dirt_Reader *reader, void *name, void *value) { return (void *) Dirt_Struct_parameter(reader->buffer->session, (Dirt_Struct *) name, (Dirt_Struct *) value); };
void *Dirt_StructReader_member(Dirt_Reader *reader, void *name, void *value) { return (void *) Dirt_Struct_member(reader->buffer->session, (Dirt_Struct *) name, (Dirt_Struct *) value); };
void *Dirt_StructReader_application(Dirt_Reader *reader, void *function, void *parameters) { return (void *) Dirt_Struct_application(reader->buffer->session, (Dirt_Struct *) function, (Dirt_Struct *) parameters); };

Dirt_Reader_Callbacks Dirt_StructReader_Callbacks = {
 Dirt_StructReader_free,
 Dirt_StructReader_str,
 Dirt_StructReader_unicodeStr,
 Dirt_StructReader_identifier,
 Dirt_StructReader_num_float,
 Dirt_StructReader_num_int,
 Dirt_StructReader_num_long,
 Dirt_StructReader_none,
 Dirt_StructReader_false,
 Dirt_StructReader_true,
 Dirt_StructReader_structure,
 Dirt_StructReader_structure_add,
 Dirt_StructReader_structure_finalize_tuple,
 Dirt_StructReader_structure_finalize_list,
 Dirt_StructReader_structure_finalize_dictionary,
 Dirt_StructReader_structure_finalize_type,
 Dirt_StructReader_keyvalue,
 Dirt_StructReader_parameter,
 Dirt_StructReader_member,
 Dirt_StructReader_application
};
