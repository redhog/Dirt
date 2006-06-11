#include "WriteBuffer.h"
#include "WriteBufferTypes.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>

#define max(x, y) (x > y ? x : y)

int main(int argc, char argv[])
 {
  Dirt_FdWriteBuffer fdbuffer;
  Dirt_WriteBuffer *buffer = (Dirt_WriteBuffer *) &fdbuffer;
  int fd[3];

  printf("Testing FileWriteBuffer\n");
  if ((fd[0] = open("WriteBufferTest.output", O_WRONLY | O_CREAT)) < 0)
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to open file");
    exit(1);
   }
  if (!Dirt_FdWriteBuffer_init(&fdbuffer, &Dirt_DebugSession, fd[0]))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to initialize FdWriteBuffer");
    exit(1);
   }
  buffer->maxsize=4;
  if (!buffer->type->extend(buffer, sizeof("foobar"))) exit(1);
  strcpy(buffer->buf + buffer->pos, "foobar");
  if (!buffer->type->advance(buffer, sizeof("foobar"))) exit(1);
  buffer->type->free(buffer);

  printf("Testing SocketWriteBuffer\n");
  if (socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Socket", strerror(errno));
    exit(1);
   }
  if (!Dirt_FdWriteBuffer_init(&fdbuffer, &Dirt_DebugSession, fd[1]))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to initialize SocketWriteBuffer");
    exit(1);
   }
  if (fork() == 0)
   {
    char dump[1024];
    ssize_t nr, wnr;

    if ((fd[2] = open("WriteBufferTest.output", O_WRONLY | O_APPEND)) < 0)
     {
      Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to open file");
      while (read(fd[0], dump, 1024) != 0);
      exit(1);
     }

    while ((nr = read(fd[0], dump, 1024)) > 0)
     {
      while ((wnr = write(fd[2], dump, nr)) > 0) nr -= wnr;
      if (nr) break;
     }
    if (nr != 0)
     {
      Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to write to file");
      while (read(fd[0], dump, 1024) != 0);
      exit(1);
     }
    close(fd[0]);
    close(fd[2]);
    exit(0);
   }
  close(fd[0]);
  buffer->maxsize=4;
  if (!buffer->type->extend(buffer, sizeof("muahehe"))) exit(1);
  strcpy(buffer->buf + buffer->pos, "muahehe");
  if (!buffer->type->advance(buffer, sizeof("muahehe"))) exit(1);
  buffer->type->free(buffer);
  exit(0);
 }
