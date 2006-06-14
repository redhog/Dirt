#include <Dirt/Reader.h>
#include <string.h>


#define NEWLINE '\n': case '\r'
#define SPACE '\t': case '\x0b': case '\x0c': case ' '
#define WHITESPACE NEWLINE: case SPACE
#define DIGITS '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'
#define NUMBERLEADCHARS DIGITS: case '+': case '-'
#define NUMBERCHARS NUMBERLEADCHARS: case '.'
#define KNOWNBEGINSEPARATORS '<': case '{': case '['
#define BEGINSEPARATORS KNOWNBEGINSEPARATORS: case '('
#define ENDSEPARATORS ')': case ']': case '}': case '>'
#define SEPARATORS ',': case ':': case '=': case BEGINSEPARATORS: case ENDSEPARATORS

ssize_t Dirt_Reader_readStr_readUnicodeescape(Dirt_Reader *reader, off_t *pos, char *dst)
 {
  off_t p = *pos;
  ssize_t len;

  switch (reader->buffer->buf[reader->buffer->pos + p++])
   {
    case 'N':
     p++; /* skip '{' too */
     {
      size_t namestart = p;

      while (1)
       {
	if (!reader->buffer->type->extend(reader->buffer, p + 1)) return -1;
	if (reader->buffer->buf[reader->buffer->pos + p] == '}')
	  break;
	p++;
       }
      {
       size_t namelen = p - namestart;
       char name[namelen + 1];

       memcpy(name, reader->buffer->buf + reader->buffer->pos + namestart, namelen);
       name[namelen] = '\0';
       p++; /* skip '}' */
       len = reader->buffer->session->type->unicodeLookupName(reader->buffer->session, name, dst);
      }
     }
     break;
    case 'u':
     {
      char num[5];

      if (!reader->buffer->type->extend(reader->buffer, p + 4)) return -1;
      memcpy(num, reader->buffer->buf + reader->buffer->pos + p, 4);
      num[4] = '\0';
      p += 4;

      len = reader->buffer->session->type->unicodeLookupOrdinal(reader->buffer->session, (int) strtol(num, NULL, 16), dst);      
     }
     break;
    case 'U':
     {
      char num[9];

      if (!reader->buffer->type->extend(reader->buffer, p + 8)) return -1;
      memcpy(num, reader->buffer->buf + reader->buffer->pos + p, 8);
      num[8] = '\0';
      p += 8;

      len = reader->buffer->session->type->unicodeLookupOrdinal(reader->buffer->session, (int) strtol(num, NULL, 16), dst);
     }
     break;
    default:
     return -1;
   }
  if (dst)
   {
    if (!reader->buffer->type->advance(reader->buffer, p)) return -1;
    p = 0;
   }
  *pos = p;
  return len;
 }

ssize_t Dirt_Reader_readStr_read(Dirt_Reader *reader, char *start, char startlen, char *dst)
 {
  char escaped = 0;
  off_t pos = 0;
  size_t len = 0;
  char c;

  while (1)
   {
    if (!reader->buffer->type->extend(reader->buffer, pos + 1)) return -1;
    c = reader->buffer->buf[reader->buffer->pos + pos];

    if (escaped)
     {
      escaped = 0;
      if (reader->buffer->type->contains(reader->buffer, start, startlen, pos))
       {
	pos += startlen;
	len += startlen;
	continue;
       }
      else 
       switch (c)
	{
	 case '\n':
	  if (dst)
	   {
	    if (!reader->buffer->type->advance(reader->buffer, 1)) return -1;
	    pos = 0;
	    continue;
	   }
	  pos++;
	  continue;
	 case 'a': case 'b': case 'f': case 'n': case 'r': case 't': case 'v':
	  len++;
	  if (!dst)
	   {
	    pos++;
	    continue;
	   }
	  switch (c)
	   {
	    case 'a': *dst = '\a'; break;
	    case 'b': *dst = '\b'; break;
	    case 'f': *dst = '\f'; break;
	    case 'n': *dst = '\n'; break;
	    case 'r': *dst = '\r'; break;
	    case 't': *dst = '\t'; break;
	    case 'v': *dst = '\v'; break;
	   }
	  dst++;
	  if (!reader->buffer->type->advance(reader->buffer, 1)) return -1;
	  pos = 0;
	  continue;
	 case 'N': case 'u': case 'U':
	  {
	   ssize_t ln;

	   ln = reader->type->readStr_readUnicodeescape(reader, &pos, dst);
	   if (ln == -1) return -1;
	   len += ln;
	   continue;
	  }
	 case 'x':
	  pos++; /* Skip x */
	  len++;
	  if (dst)
	   {
	    char num[3];

	    if (!reader->buffer->type->extend(reader->buffer, pos + 2)) return -1;
	    memcpy(num, reader->buffer->buf + reader->buffer->pos + pos, 2);
	    num[2] = '\0';

	    *((unsigned char *) dst++) = strtol(num, NULL, 16);
	    if (!reader->buffer->type->advance(reader->buffer, pos + 2)) return -1;
	    pos = 0;
	   }
	  else
	   pos += 2;
	  continue;
	 case DIGITS:
	  len++;
	  if (dst)
	   {
	    char num[4];

	    if (!reader->buffer->type->extend(reader->buffer, pos + 3)) return -1;
	    memcpy(num, reader->buffer->buf + reader->buffer->pos + pos, 3);
	    num[3] = '\0';

	    *((unsigned char *) dst++) = strtol(num, NULL, 8);
	    if (!reader->buffer->type->advance(reader->buffer, pos + 3)) return -1;
	    pos = 0;
	   }
	  else
	    pos += 3;
	  continue;
	}
     }
    else
     {
      if (reader->buffer->type->contains(reader->buffer, start, startlen, pos))
       {
	if (!dst)
	 break;
	if (pos) if (!reader->buffer->type->cut(reader->buffer, dst, pos)) return -1;
	if (!reader->buffer->type->advance(reader->buffer, startlen)) return -1;
	break;
       }
      else if (c == '\\')
       {
	if (dst)
	 {
	  if (pos) if (!reader->buffer->type->cut(reader->buffer, dst, pos)) return -1;
	  dst += pos;
	  if (!reader->buffer->type->advance(reader->buffer, 1)) return -1; /* Skip '\' */
	  pos = 0;
	 }
	else
	 pos++;
        escaped = 1;
	continue;
       }
     }
    len++;
    pos++;
   }
  return len;
 }

void *Dirt_Reader_readStr(Dirt_Reader *reader, char unicode)
 {
  char start[3];
  char startlen = 1;
  size_t len = 0;
  void *res;

  if (!reader->buffer->type->extend(reader->buffer, 2)) return NULL;
  start[0] = start[1] = start[2] = reader->buffer->buf[reader->buffer->pos];
  if (!reader->buffer->type->advance(reader->buffer, 1)) return NULL;
  if (reader->buffer->type->contains(reader->buffer, start, 2, 0))
   {
    startlen = 3;
    if (!reader->buffer->type->advance(reader->buffer, 2)) return NULL;
   }
  if ((len = reader->type->readStr_read(reader, start, startlen, NULL)) == -1) return NULL;
  {
   char buf[len + 1];

   if (reader->type->readStr_read(reader, start, startlen, buf) == -1) return NULL;
   buf[len] = '\0';

   if (unicode)
    return reader->callback->unicodeStr(reader, buf, len);
   else
    return reader->callback->str(reader, buf, len);
   return res;
  }
 }

void *Dirt_Reader_readNum(Dirt_Reader *reader)
 {
  void *res;
  off_t pos = 0;
  char isf = 0;
  char isl = 0;
  char c;

  while (1)
   {
    if (!reader->buffer->type->extend(reader->buffer, pos + 1))
     break;
    c = reader->buffer->buf[reader->buffer->pos + pos];
    switch (c)
     {
      case SEPARATORS:
      case WHITESPACE:
       goto Dirt_readNum_end;
      case 'F': case 'f': case 'E': case 'e': case '.':
       isf = 1;
       break;
      case 'L': case 'l':
       isl = 1;
       break;
     }
    pos++;
   }
  Dirt_readNum_end:
  {
   char numstr[pos + 1];

   reader->buffer->type->cut(reader->buffer, numstr, pos); /* Nah, we've allready done extend on this, so this *can't* fail */
   numstr[pos] = '\0';

   if (isf)
    return reader->callback->num_float(reader, atof(numstr));
   else if (isl)
    return reader->callback->num_long(reader, atol(numstr));
   else
    return reader->callback->num_int(reader, atoi(numstr));
   return res;
  }
 }

ssize_t Dirt_Reader_readIdentifier_read(Dirt_Reader *reader, char *dst)
 {
  ssize_t len = 0;
  off_t pos = 0;
  off_t escape = 0;
  char c;

  while (1)
   {
    if (!reader->buffer->type->extend(reader->buffer, pos + 1))
     goto Dirt_readIdentifier_read_end;
    c = reader->buffer->buf[reader->buffer->pos + pos];
    if (!escape)
     switch (c)
      {
       case '.': case SEPARATORS: case WHITESPACE:
        goto Dirt_readIdentifier_read_end;
       case '\\':
        escape = 1;
	if (dst)
	 {
	  if (pos) if (!reader->buffer->type->cut(reader->buffer, dst, pos)) return -1;
	  dst += pos;
	  if (!reader->buffer->type->advance(reader->buffer, 1)) return -1;
	  pos = 0;
	  continue;
	 }
	pos++;
	continue;
      }
    escape = 0;
    pos++;
    len++;
   }
  Dirt_readIdentifier_read_end:
   if (dst)
    if (pos) if (!reader->buffer->type->cut(reader->buffer, dst, pos)) return -1;
   return len;
 }

void *Dirt_Reader_readIdentifier(Dirt_Reader *reader)
 {
  off_t len = 0;

  if (!(len = reader->type->readIdentifier_read(reader, NULL))) return NULL;
  {
   char identifier[len + 1];

   if (!(len = reader->type->readIdentifier_read(reader, identifier))) return NULL;
   identifier[len] = '\0';

   if (strcmp(identifier, "None") == 0)
    return reader->callback->none(reader);
   else if (strcmp(identifier, "True") == 0)
    return reader->callback->true(reader);
   else if (strcmp(identifier, "False") == 0)
    return reader->callback->false(reader);
   else
    return reader->callback->identifier(reader, identifier, len);
  }
 }

void *Dirt_Reader_readIdentifiers(Dirt_Reader *reader)
 {
  void *res;
  void *item;

  if (!(res = reader->type->readIdentifier(reader))) return NULL;
  if (!reader->buffer->type->contains(reader->buffer, ".", 1, 0))
   return res;
  if (!reader->buffer->type->advance(reader->buffer, 1))
   return NULL;
  if (!(item = reader->type->readIdentifiers(reader))) return NULL;
  return reader->callback->member(reader, res, item);
 }

void *Dirt_Reader_readUnicodeOrId(Dirt_Reader *reader)
 {
  if (   reader->buffer->type->contains(reader->buffer, "u'", 2, 0)
      || reader->buffer->type->contains(reader->buffer, "u\"", 2, 0)
      || reader->buffer->type->contains(reader->buffer, "U'", 2, 0)
      || reader->buffer->type->contains(reader->buffer, "U\"", 2, 0))
   {    
    reader->buffer->type->advance(reader->buffer, 1);
    return reader->type->readStr(reader, 1);
   }
  else
   return reader->type->readIdentifiers(reader);
 }

void *Dirt_Reader_read(Dirt_Reader *reader, char inParens)
 {
  void *res;
  void *last = NULL; /* last contains the item that should have been at the end of res */
  void *value = NULL;
  size_t items = 0;
  char separated = 1;
  char c;

  if (!(res = reader->callback->structure(reader))) return NULL;
  while (1)
   {
    if (!reader->buffer->type->extend(reader->buffer, 1))
     goto Dirt_internalRead_end;
    c = reader->buffer->buf[reader->buffer->pos];
    switch (c)
     {
      case NEWLINE:
       if (!items || inParens)
	continue;
       goto Dirt_internalRead_end;
      case ENDSEPARATORS:
       if (inParens)
	if (!reader->buffer->type->advance(reader->buffer, 1)) goto Dirt_internalRead_error;
       goto Dirt_internalRead_end;
      case SPACE:
       if (!reader->buffer->type->advance(reader->buffer, 1)) goto Dirt_internalRead_error;
       continue;
      case ',':
       if (!inParens)
	goto Dirt_internalRead_end;
       if (!reader->buffer->type->advance(reader->buffer, 1)) goto Dirt_internalRead_error;
       separated = 1;
       continue;
      case ':':
       if (inParens != '{')
	reader->buffer->session->type->error(reader->buffer->session, "Syntax error", "Colon not inside a mapping");
       if (!reader->buffer->type->advance(reader->buffer, 1)) goto Dirt_internalRead_error;
       if (!(value = reader->type->read(reader, 0))) goto Dirt_internalRead_error;
       last = reader->callback->keyvalue(reader, last, value);
       continue;
      case '=':
       if (!reader->buffer->type->advance(reader->buffer, 1)) goto Dirt_internalRead_error;
       if (!(value = reader->type->read(reader, 0))) goto Dirt_internalRead_error;
       last = reader->callback->parameter(reader, last, value);
       continue;
      case '(':
       if (!reader->buffer->type->advance(reader->buffer, 1)) goto Dirt_internalRead_error;
       if (!(value = reader->type->read(reader, separated ? '(' : '['))) goto Dirt_internalRead_error;
       if (!separated)
	{
	 last = reader->callback->application(reader, last, value);
	 continue;
	}
       break;
      case KNOWNBEGINSEPARATORS:
       if (!reader->buffer->type->advance(reader->buffer, 1)) goto Dirt_internalRead_error;
       if (!(value = reader->type->read(reader, c))) goto Dirt_internalRead_error;
       break;
      case '\'': case '"':
       value = reader->type->readStr(reader, 0);
       if (!value) goto Dirt_internalRead_error;
       break;
      case NUMBERLEADCHARS:
       value = reader->type->readNum(reader);
       if (!value) goto Dirt_internalRead_error;
       break;
      default:
       value = reader->type->readUnicodeOrId(reader);
       if (!value) goto Dirt_internalRead_error;
       break;
     }
    separated = 0;
    if (items)
     if (!(res = reader->callback->structure_add(reader, res, last))) goto Dirt_internalRead_error;
    last = value;
    items++;
   }

  Dirt_internalRead_end:
   if (!inParens)
    {
     if (!items)
      {
       reader->buffer->session->type->error(reader->buffer->session, Dirt_Buffer_Error_IO, "No expression found");
       goto Dirt_internalRead_error;
      }
     reader->callback->free(reader, res);
     return last;
    }
   if (inParens == '(')
     if (items == 1)
      {
       reader->callback->free(reader, res);
       return last;
      }
   if (items)
    if (!(res = reader->callback->structure_add(reader, res, last)))
     goto Dirt_internalRead_error;
   switch (inParens)
    {
     case '(':
      if (!(res = reader->callback->structure_finalize_tuple(reader, res)))
       goto Dirt_internalRead_error;
      break;
     case '[':
      if (!(res = reader->callback->structure_finalize_list(reader, res)))
       goto Dirt_internalRead_error;
      break;
     case '{':
      if (!(res = reader->callback->structure_finalize_dictionary(reader, res)))
       goto Dirt_internalRead_error;
      break;
     case '<':
      if (!(res = reader->callback->structure_finalize_type(reader, res)))
       goto Dirt_internalRead_error;
      break;
    }
   return res;
  Dirt_internalRead_error:
   reader->buffer->session->type->error(reader->buffer->session, "Reader", "Reader error");
   reader->callback->free(reader, res);
   reader->callback->free(reader, last);
   return NULL;
 }

Dirt_ReaderType Dirt_StandardReaderType = {
 Dirt_Reader_readStr_readUnicodeescape,
 Dirt_Reader_readStr_read,
 Dirt_Reader_readStr,
 Dirt_Reader_readNum,
 Dirt_Reader_readIdentifier_read,
 Dirt_Reader_readIdentifier,
 Dirt_Reader_readIdentifiers,
 Dirt_Reader_readUnicodeOrId,
 Dirt_Reader_read
};

char Dirt_StandardReader_init(Dirt_Reader *reader, Dirt_Reader_Callbacks *callbacks, Dirt_Buffer *buffer)
 {
  reader->type = &Dirt_StandardReaderType;
  reader->callback = callbacks;
  reader->buffer = buffer;
 }
