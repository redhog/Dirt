#include <Dirt/WriteBufferImplementor.h>
#include <Dirt/WriteBufferTypes.h>
#include <Dirt/Utils.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>


/****************************************************************
 StringWriteBuffer
 ****************************************************************/

Dirt_WriteBufferType Dirt_StringWriteBufferType;

char Dirt_StringWriteBuffer_init(Dirt_WriteBuffer *buffer, Dirt_Session *session)
 {
  if (!Dirt_WriteBuffer_init(buffer, &Dirt_StringWriteBufferType, session)) return 0;
  buffer->type = &Dirt_StringWriteBufferType;
 }

void Dirt_StringWriteBuffer_free(Dirt_WriteBuffer *buffer)
 {
  free(buffer->buf);
 }

static char Dirt_StringWriteBuffer_adjust(Dirt_WriteBuffer *buffer)
 {
  return 1;
 }

Dirt_WriteBufferType Dirt_StringWriteBufferType = {
 NULL,

 Dirt_StringWriteBuffer_free,

 NULL,
 NULL,

 NULL,
 &Dirt_StringWriteBuffer_adjust,
 &Dirt_WriteBuffer_advance,
 &Dirt_WriteBuffer_extend
};


/****************************************************************
 FdWriteBuffer
 ****************************************************************/

Dirt_WriteBufferType Dirt_FdWriteBufferType;

void Dirt_FdWriteBuffer_free(Dirt_WriteBuffer *buffer)
 {

  Dirt_WriteBuffer_free(buffer);
  close(((Dirt_FdWriteBuffer *) buffer)->fd);
 }

char Dirt_FdWriteBuffer_init(Dirt_FdWriteBuffer *buffer, Dirt_Session *session, int fd)
 {
  if (!Dirt_WriteBuffer_init(&buffer->buffer, &Dirt_FdWriteBufferType, session)) return 0;
  buffer->fd = fd;
  return 1; 
 }

char Dirt_FdWriteBuffer_write(Dirt_WriteBuffer *buffer, size_t nr)
 {
  ssize_t res;
  size_t endpos = min(buffer->wpos + nr, buffer->pos);
  
  while (   endpos - buffer->wpos
	 && (res = write(((Dirt_FdWriteBuffer *) buffer)->fd,
			 buffer->buf + buffer->wpos,
			 endpos - buffer->wpos)) > 0)
   buffer->wpos += res;
  if (endpos - buffer->wpos)
   {
    /* #### fixme ####
       # name = 'Not threadsafe'
       # description = 'Use strerror_r or something instaed...'
       #### end ####
    */
    buffer->session->type->error(buffer->session, Dirt_WriteBuffer_Error_IO, strerror(errno));
    return 0;
   }
  return 1;
 }

Dirt_WriteBufferType Dirt_FdWriteBufferType = {
 NULL,

 &Dirt_FdWriteBuffer_free,

 NULL,
 NULL,

 &Dirt_FdWriteBuffer_write,
 &Dirt_WriteBuffer_adjust,
 &Dirt_WriteBuffer_advance,
 &Dirt_WriteBuffer_extend
};
