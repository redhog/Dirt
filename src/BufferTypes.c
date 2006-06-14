#include <Dirt/BufferTypes.h>
#include <Dirt/BufferImplementor.h>
#include <Dirt/Utils.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/****************************************************************
 StringBuffer
 ****************************************************************/

Dirt_BufferType Dirt_StringBufferType;

char Dirt_StringBuffer_init(Dirt_Buffer *buffer, Dirt_Session *session, char *string)
 {
  if (!Dirt_Buffer_init(buffer, &Dirt_StringBufferType, session)) return 0;
  buffer->type = &Dirt_StringBufferType;
  buffer->buf = string;
  buffer->len = buffer->size = strlen(string);
 }

static char Dirt_StringBuffer_extend(Dirt_Buffer *buffer, size_t nr)
 {
  if (nr <= buffer->len)
   return 1;
  
  buffer->session->type->error(buffer->session, Dirt_Buffer_Error_IO, "Premature end of file reached");
  return 0;
 }

Dirt_BufferType Dirt_StringBufferType = {
 NULL,

 NULL,

 NULL,
 NULL,

 &Dirt_Buffer_advance,
 &Dirt_Buffer_cut,
 &Dirt_Buffer_contains,
 NULL,
 &Dirt_StringBuffer_extend
};


/****************************************************************
 FdBuffer
 ****************************************************************/

void Dirt_FdBuffer_free(Dirt_Buffer *buffer)
 {
  close(((Dirt_FdBuffer *) buffer)->fd);
  Dirt_Buffer_free(buffer);
 }



/****************************************************************
 SocketBuffer
 ****************************************************************/

Dirt_BufferType Dirt_SocketBufferType;

char Dirt_SocketBuffer_init(Dirt_FdBuffer *buffer, Dirt_Session *session, int fd)
 {
  struct stat st;

  if (fstat(fd, &st) == -1)
   return 0;
  if (!S_ISSOCK(st.st_mode))
   return 0;
  if (!Dirt_Buffer_init(&buffer->buffer, &Dirt_SocketBufferType, session)) return 0;
  buffer->fd = fd;
  return 1;
 }

ssize_t Dirt_SocketBuffer_read(Dirt_Buffer *buffer, size_t nr)
 {
  ssize_t res;
# ifdef DEBUG_READ
   fprintf(stderr, "read(%i) -> "); fflush(stderr);
# endif
  res = recv(((Dirt_FdBuffer *) buffer)->fd,
	     buffer->buf + buffer->pos + buffer->len,
	     max(nr, buffer->maxsize),
	     0);
# ifdef DEBUG_READ
  if (res < 0)
   perror("returned error");
  else
   {
    fprintf(stderr, "%i >", (int) res);
    fwrite(buffer->buf + buffer->pos + buffer->len, res, 1, stderr);
    fprintf(stderr, "<\n");
    fflush(stderr);
   }
# endif
  return res;
 }

Dirt_BufferType Dirt_SocketBufferType = {
 NULL,

 &Dirt_FdBuffer_free,

 NULL,
 NULL,

 &Dirt_Buffer_advance,
 &Dirt_Buffer_cut,
 &Dirt_Buffer_contains,
 &Dirt_SocketBuffer_read,
 &Dirt_Buffer_extend
};


/****************************************************************
 FileBuffer
 ****************************************************************/

Dirt_BufferType Dirt_FileBufferType;

char Dirt_FileBuffer_init(Dirt_FdBuffer *buffer, Dirt_Session *session, int fd)
 {
  if (!Dirt_Buffer_init(&buffer->buffer, &Dirt_FileBufferType, session)) return 0;
  buffer->fd = fd;
  return 1; 
 }

ssize_t Dirt_FileBuffer_read(Dirt_Buffer *buffer, size_t nr)
 {
  return read(((Dirt_FdBuffer *) buffer)->fd,
	      buffer->buf + buffer->pos + buffer->len,
	      nr - buffer->len);
 }

Dirt_BufferType Dirt_FileBufferType = {
 NULL,

 &Dirt_FdBuffer_free,

 NULL,
 NULL,

 &Dirt_Buffer_advance,
 &Dirt_Buffer_cut,
 &Dirt_Buffer_contains,
 &Dirt_FileBuffer_read,
 &Dirt_Buffer_extend
};
