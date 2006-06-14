#include <Dirt/WriteBufferImplementor.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#if 0
# define inline
# define DEBUG_WRITE
# define DEBUG_THREAD
# define DEBUG_TRAVERSE
# define DEBUG_ALLOC
#endif 


char Dirt_WriteBuffer_Error_IO[] = "I/O error";

/* Generic implementations */

void Dirt_WriteBuffer_free(Dirt_WriteBuffer *buffer)
 {
  buffer->type->write(buffer, buffer->pos - buffer->wpos);
  free(buffer->buf);
 }

char Dirt_WriteBuffer_adjust(Dirt_WriteBuffer *buffer)
 {
  if (buffer->pos - buffer->wpos > buffer->maxsize)
   {
    if (!buffer->type->write(buffer, (buffer->pos - buffer->wpos) - buffer->maxsize))
     return 0;
   }
  if (buffer->wpos >= buffer->maxsize)
   {
    memmove(buffer->buf, buffer->buf + buffer->wpos, buffer->pos + buffer->size - buffer->wpos);
    buffer->pos -= buffer->wpos;
    buffer->size = buffer->wpos;
    buffer->wpos = 0;
    if (!(buffer->buf = realloc(buffer->buf, buffer->pos + buffer->size)))
     return 0;
   }
  return 1;
}

char Dirt_WriteBuffer_advance(Dirt_WriteBuffer *buffer, size_t nr)
 {
  if (nr > buffer->size)
   {
    return 0;
   }
  buffer->pos += nr;
  buffer->size -= nr;
  return buffer->type->adjust(buffer);
 }

char Dirt_WriteBuffer_extend(Dirt_WriteBuffer *buffer, size_t nr)
 {
  ssize_t w;

  if (nr <= buffer->size)
   return 1;
  if (!buffer->type->adjust(buffer)) return 0;
  buffer->buf = realloc(buffer->buf, buffer->pos + nr);
  buffer->size = nr;
  return 1;
 }

char Dirt_WriteBuffer_init(Dirt_WriteBuffer *buffer, Dirt_WriteBufferType *type, Dirt_Session *session)
 {
  uint maxsize = NEWREADER_WRITE_BUFFER_MAXSIZE;
# ifdef DEBUG_ALLOC
   fprintf(stderr, "Init: %p\n", (void *) buffer);
# endif
  buffer->buf = NULL;
  buffer->pos = 0;
  buffer->wpos = 0;
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
