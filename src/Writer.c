#include <Dirt/Writer.h>
#include <Dirt/ReaderImplementor.h>
#include <string.h>
#include <stdio.h>

char Dirt_Writer_writeBytes(Dirt_Writer *writer, char *bytes, size_t len)
 {
  if (!writer->buffer->type->extend(writer->buffer, len)) return 0;
  memcpy(writer->buffer->buf + writer->buffer->pos, bytes, len);
  if (!writer->buffer->type->advance(writer->buffer, len)) return 0;
  return 1;
 }

char Dirt_Writer_writeStructure(Dirt_Writer *writer, void *structure)
 {
  void *iter;
  
  if (!(iter = writer->callback->structure_begin(writer, structure))) return 0;
  while (!writer->callback->structure_end(writer, iter))
   {
    if (!writer->callback->write(writer, writer->callback->structure_next(writer, iter))) return 0;
    if (!Dirt_Writer_writeBytes(writer, ", ", sizeof(", ") - 1)) return 0;
   }
  return 1;
 }

char Dirt_Writer_str(Dirt_Writer *writer, char *str, size_t len)
 {
  size_t unescaped;

  if (!writer->buffer->type->extend(writer->buffer, len + 2)) return 0;
  writer->buffer->buf[writer->buffer->pos] = '\'';
  if (!writer->buffer->type->advance(writer->buffer, 1)) return 0;

  while (len)
   {
    for (unescaped = 0;
	 unescaped < len
	 && str[unescaped] != '\''
	 && str[unescaped] != '\n'
	 && str[unescaped] != '\\';
	 unescaped++);

    if (!writer->buffer->type->extend(writer->buffer, unescaped + 2)) return 0;
    memcpy(writer->buffer->buf + writer->buffer->pos, str, unescaped);
    if (!writer->buffer->type->advance(writer->buffer, unescaped)) return 0;
    str += unescaped;
    len -= unescaped;
    if (unescaped < len)
     {
      writer->buffer->buf[writer->buffer->pos] = '\\';
      writer->buffer->buf[writer->buffer->pos + 1] = str[0];
      if (!writer->buffer->type->advance(writer->buffer, 2)) return 0;
      str += 1;
      len -= 1;
     }
   }
  if (!writer->buffer->type->extend(writer->buffer, 1)) return 0;
  writer->buffer->buf[writer->buffer->pos] = '\'';
  if (!writer->buffer->type->advance(writer->buffer, 1)) return 0;
  return 1;
 }

char Dirt_Writer_unicodeStr(Dirt_Writer *writer, char *str, size_t len)
 {
  if (!writer->buffer->type->extend(writer->buffer, 1)) return 0;
  writer->buffer->buf[writer->buffer->pos] = 'u';
  if (!writer->buffer->type->advance(writer->buffer, 1)) return 0;
  return Dirt_Writer_str(writer, str, len);
 }

char Dirt_Writer_identifier(Dirt_Writer *writer, char *identifier, size_t len)
 {
  size_t unescaped;

  if (!writer->buffer->type->extend(writer->buffer, len)) return 0;

  while (len)
   {
    for (unescaped = 0;
	 unescaped < len;
	 unescaped++)
     switch (identifier[unescaped])
      {
       case '.': case SEPARATORS: case WHITESPACE:
	goto Dirt_Writer_identifier_escape;
      }
    Dirt_Writer_identifier_escape:
     if (!writer->buffer->type->extend(writer->buffer, unescaped + 2)) return 0;
     memcpy(writer->buffer->buf + writer->buffer->pos, identifier, unescaped);
     if (!writer->buffer->type->advance(writer->buffer, unescaped)) return 0;
     identifier += unescaped;
     len -= unescaped;
     if (unescaped < len)
      {
       writer->buffer->buf[writer->buffer->pos] = '\\';
       writer->buffer->buf[writer->buffer->pos + 1] = identifier[0];
       if (!writer->buffer->type->advance(writer->buffer, 2)) return 0;
       identifier += 1;
       len -= 1;
      }
   }
  return 1;
 }
 
char Dirt_Writer_num_float(Dirt_Writer *writer, float real)
 {
  size_t numlen;

  numlen = snprintf(NULL, 0, "%f", real);

  /* + 1 since snprintf will want to output a superflous \0 at the end... */
  if (!writer->buffer->type->extend(writer->buffer, numlen + 1)) return 0;
  snprintf(writer->buffer->buf + writer->buffer->pos, numlen + 1, "%f", real);
  if (!writer->buffer->type->advance(writer->buffer, numlen)) return 0;
  return 1;
 }

char Dirt_Writer_num_long(Dirt_Writer *writer, long integer)
 {
  size_t numlen;

  numlen = snprintf(NULL, 0, "%l", integer);

  /* + 1 since snprintf will want to output a superflous \0 at the end... */
  if (!writer->buffer->type->extend(writer->buffer, numlen + 1)) return 0;
  snprintf(writer->buffer->buf + writer->buffer->pos, numlen + 1, "%l", integer);
  if (!writer->buffer->type->advance(writer->buffer, numlen)) return 0;
  return 1;
 }
 
char Dirt_Writer_num_int(Dirt_Writer *writer, int integer)
 {
  size_t numlen;

  numlen = snprintf(NULL, 0, "%i", integer);

  /* + 1 since snprintf will want to output a superflous \0 at the end... */
  if (!writer->buffer->type->extend(writer->buffer, numlen + 1)) return 0;
  snprintf(writer->buffer->buf + writer->buffer->pos, numlen + 1, "%i", integer);
  if (!writer->buffer->type->advance(writer->buffer, numlen)) return 0;
  return 1;
 }

char Dirt_Writer_none(Dirt_Writer *writer)
 {
  return Dirt_Writer_writeBytes(writer, "None", sizeof("None") - 1);
 }

char Dirt_Writer_false(Dirt_Writer *writer)
 {
  return Dirt_Writer_writeBytes(writer, "False", sizeof("False") - 1);
 }

char Dirt_Writer_true(Dirt_Writer *writer)
 {
  return Dirt_Writer_writeBytes(writer, "True", sizeof("True") - 1);
 }

char Dirt_Writer_structure_tuple(Dirt_Writer *writer, void *structure)
 {
  if (!Dirt_Writer_writeBytes(writer, "(", sizeof("(") - 1)) return 0;
  if (!Dirt_Writer_writeStructure(writer, structure)) return 0;
  if (!Dirt_Writer_writeBytes(writer, ")", sizeof(")") - 1)) return 0;
  return 1;
 }

char Dirt_Writer_structure_list(Dirt_Writer *writer, void *structure)
 {
  if (!Dirt_Writer_writeBytes(writer, "[", sizeof("[") - 1)) return 0;
  if (!Dirt_Writer_writeStructure(writer, structure)) return 0;
  if (!Dirt_Writer_writeBytes(writer, "]", sizeof("]") - 1)) return 0;
  return 1;
 }

char Dirt_Writer_structure_dictionary(Dirt_Writer *writer, void *structure)
 {
  if (!Dirt_Writer_writeBytes(writer, "{", sizeof("{") - 1)) return 0;
  if (!Dirt_Writer_writeStructure(writer, structure)) return 0;
  if (!Dirt_Writer_writeBytes(writer, "}", sizeof("}") - 1)) return 0;
  return 1;
 }

char Dirt_Writer_structure_type(Dirt_Writer *writer, void *structure)
 {
  if (!Dirt_Writer_writeBytes(writer, "<", sizeof("<") - 1)) return 0;
  if (!Dirt_Writer_writeStructure(writer, structure)) return 0;
  if (!Dirt_Writer_writeBytes(writer, ">", sizeof(">") - 1)) return 0;
  return 1;
 }

char Dirt_Writer_keyvalue(Dirt_Writer *writer, void *keyvalue)
 {
  if (!writer->callback->write(writer, writer->callback->keyvalue_key(writer, keyvalue))) return 0;
  if (!Dirt_Writer_writeBytes(writer, ": ", sizeof(": ") - 1)) return 0;
  if (!writer->callback->write(writer, writer->callback->keyvalue_value(writer, keyvalue))) return 0;
  return 1;
 }

char Dirt_Writer_parameter(Dirt_Writer *writer, void *keyvalue)
 {
  if (!writer->callback->write(writer, writer->callback->keyvalue_key(writer, keyvalue))) return 0;
  if (!Dirt_Writer_writeBytes(writer, " = ", sizeof(" = ") - 1)) return 0;
  if (!writer->callback->write(writer, writer->callback->keyvalue_value(writer, keyvalue))) return 0;
  return 1;
 }

char Dirt_Writer_member(Dirt_Writer *writer, void *keyvalue)
 {
  if (!writer->callback->write(writer, writer->callback->keyvalue_key(writer, keyvalue))) return 0;
  if (!Dirt_Writer_writeBytes(writer, ".", sizeof(".") - 1)) return 0;
  if (!writer->callback->write(writer, writer->callback->keyvalue_value(writer, keyvalue))) return 0;
  return 1;
 }

char Dirt_Writer_application(Dirt_Writer *writer, void *keyvalue)
 {
  if (!writer->callback->write(writer, writer->callback->keyvalue_key(writer, keyvalue))) return 0;
  if (!Dirt_Writer_writeBytes(writer, "(", sizeof("(") - 1)) return 0;
  if (!writer->callback->write(writer, writer->callback->keyvalue_value(writer, keyvalue))) return 0;
  if (!Dirt_Writer_writeBytes(writer, ")", sizeof(")") - 1)) return 0;
  return 1;
 }


Dirt_WriterType Dirt_StandardWriterType = {
 Dirt_Writer_str,
 Dirt_Writer_unicodeStr,
 Dirt_Writer_identifier,
 Dirt_Writer_num_float,
 Dirt_Writer_num_int,
 Dirt_Writer_num_long,
 Dirt_Writer_none,
 Dirt_Writer_false,
 Dirt_Writer_true,
 Dirt_Writer_structure_tuple,
 Dirt_Writer_structure_list,
 Dirt_Writer_structure_dictionary,
 Dirt_Writer_structure_type,
 Dirt_Writer_keyvalue,
 Dirt_Writer_parameter,
 Dirt_Writer_member,
 Dirt_Writer_application,
};

char Dirt_StandardWriter_init(Dirt_Writer *writer, Dirt_Writer_Callbacks *callbacks, Dirt_WriteBuffer *buffer)
 {
  writer->type = &Dirt_StandardWriterType;
  writer->callback = callbacks;
  writer->buffer = buffer;
 }
