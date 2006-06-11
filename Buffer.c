#include "BufferImplementor.h"
#include "Utils.h"
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#if 0
# define inline
# define DEBUG_READ
# define DEBUG_THREAD
# define DEBUG_TRAVERSE
# define DEBUG_ALLOC
#endif 


char Dirt_Buffer_Error_IO[] = "I/O error";

/* Generic implementations */

void Dirt_Buffer_free(Dirt_Buffer *buffer)
 {
  free(buffer->buf);
 }

char Dirt_Buffer_advance(Dirt_Buffer *buffer, size_t nr) 
 {
  if (!buffer->type->extend(buffer, nr))
   return 0;
  buffer->pos += nr;
  buffer->len -= nr;
  return 1;
 }

char Dirt_Buffer_cut(Dirt_Buffer *buffer, char *dst, size_t nr)
 {
  if (!nr)
   nr = buffer->len;
  else
   if (!buffer->type->extend(buffer, nr))
    return 0;
  strncpy(dst, buffer->buf + buffer->pos, nr);
  buffer->type->advance(buffer, nr);
  return 1;
 }

char Dirt_Buffer_contains(Dirt_Buffer *buffer, char *buf, size_t len, size_t at)
 {
  off_t pos;
  off_t rpos;

  for (pos = 0; pos < len; pos++)
   {
    rpos = at + pos;
    if (!buffer->type->extend(buffer, rpos + 1))
     return 0;
    if (buffer->buf[buffer->pos + rpos] != buf[pos])
     return 0;
   }
  return 1;  
 }

char Dirt_Buffer_extend(Dirt_Buffer *buffer, size_t nr)
 {
  ssize_t r;

  if (nr <= buffer->len)
   return 1;
  if (buffer->pos >= buffer->maxsize)
   {
    memmove(buffer->buf, buffer->buf + buffer->pos, buffer->len);
    buffer->pos = 0;
   }
  if (   buffer->size > max(nr, buffer->maxsize) + buffer->maxsize
      || buffer->size < max(nr, buffer->maxsize))
   {
    buffer->size = max(nr, buffer->maxsize) + buffer->maxsize;
    buffer->buf = realloc(buffer->buf, buffer->pos + buffer->size);
   }
  while (nr > buffer->len)
   {
    r = buffer->type->read(buffer, nr);
    if (!r || r == -1)
     break;
    buffer->len += r;
   }
  if (nr > buffer->len)
   {
    buffer->session->type->error(buffer->session, Dirt_Buffer_Error_IO, "Premature end of file reached");
    return 0;
   }
  return 1;
 }

char Dirt_Buffer_init(Dirt_Buffer *buffer, Dirt_BufferType *type, Dirt_Session *session)
 {
  uint maxsize = NEWREADER_BUFFER_MAXSIZE;
# ifdef DEBUG_ALLOC
   fprintf(stderr, "Init: %p\n", (void *) buffer);
# endif
  buffer->buf = NULL;
  buffer->pos = 0;
  buffer->len = 0;
  buffer->size = 0;
  buffer->maxsize = maxsize;
  buffer->type = type;
  buffer->session = session;
# ifdef WITH_THREAD
#  ifdef DEBUG_THREAD
    buffer->interplockings = 0;
    buffer->bufferlockings = 0;
#  endif
# endif
# ifdef DEBUG_ALLOC
   fprintf(stderr, "Init done: %p\n", (void *) buffer);
# endif
  return 1;
 }
