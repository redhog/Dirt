#include <Dirt/Buffer.h>
#include <Dirt/BufferTypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define max(x, y) (x > y ? x : y)

void assertBuffer(Dirt_Buffer *buffer, char *string)
 {
  size_t stringlen = strlen(string);

  if (stringlen <= buffer->len && (strncmp(buffer->buf + buffer->pos, string, stringlen) != 0))
   {
    printf("Bad buffer contents '%.*s' (should have been '%s')\n", buffer->len, buffer->buf + buffer->pos, string);
    exit(1);
   }
 }

int main(int argc, char **argv)
 {
  Dirt_FdBuffer fdbuffer;
  Dirt_Buffer *buffer = (Dirt_Buffer *) &fdbuffer;
  int fd[2];

  printf("Testing FileBuffer\n");
  if ((fd[0] = open("BufferTest.c", O_RDONLY)) < 0)
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to open file");
    exit(1);
   }
  if (!Dirt_FileBuffer_init(&fdbuffer, &Dirt_DebugSession, fd[0]))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to initialize FileBuffer");
    exit(1);
   }
  buffer->type->extend(buffer, 10);
  assertBuffer(buffer, "#include <");
  buffer->type->advance(buffer, 5);
  assertBuffer(buffer, "ude <");
  buffer->type->extend(buffer, 10);
  assertBuffer(buffer, "ude <Dirt/");
  buffer->type->free(buffer);

  printf("Testing SocketBuffer\n");
  if (socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Socket", strerror(errno));
    exit(1);
   }
  if (!Dirt_SocketBuffer_init(&fdbuffer, &Dirt_DebugSession, fd[0]))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to initialize SocketBuffer");
    exit(1);
   }
  if (fork() == 0)
   {
    FILE *file = fdopen(fd[1], "w");
    fprintf(file, "foobarfienajahehe");
    fflush(file);
    fclose(file);
    exit(0);
   }
  buffer->type->extend(buffer, 10);
  assertBuffer(buffer, "foobarfien");
  buffer->type->advance(buffer, 5);
  assertBuffer(buffer, "rfien");
  buffer->type->extend(buffer, 10);
  assertBuffer(buffer, "rfienajahe");
  buffer->type->free(buffer);
  exit(0);
 }
