#include <Dirt/Writer.h>
#include <Dirt/WriteBufferTypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

int listlen = 1;

char  WriterTest_write(Dirt_Writer *writer, void *any) { listlen *= 2; return writer->type->num_int(writer, listlen); };
void *WriterTest_structure_open(Dirt_Writer *writer, void *structure) { return (void *) 1; };
char  WriterTest_structure_end(Dirt_Writer *writer, void *iter) { return listlen >= 10000; };
void *WriterTest_structure_next(Dirt_Writer *writer, void *iter) { return (void *) 1; };
void  WriterTest_structure_close(Dirt_Writer *writer, void *iter) { };
void *WriterTest_keyvalue_key(Dirt_Writer *writer, void *keyvalue) { return 0; };
void *WriterTest_keyvalue_value(Dirt_Writer *writer, void *keyvalue) { return 0; };

Dirt_Writer_Callbacks callbacks = {
 WriterTest_write,
 WriterTest_structure_open,
 WriterTest_structure_end,
 WriterTest_structure_next,
 WriterTest_structure_close,
 WriterTest_keyvalue_key,
 WriterTest_keyvalue_value
};

int main(int argc, char argv[])
 {
  Dirt_FdWriteBuffer fdbuffer;
  Dirt_WriteBuffer *buffer = (Dirt_WriteBuffer *) &fdbuffer;
  Dirt_Writer writer;
  int fd;

  if ((fd = open("WriterTest.output", O_WRONLY | O_CREAT)) < 0)
   {
    perror("Unable to open file");
    exit(1);
   }
  if (!Dirt_FdWriteBuffer_init(&fdbuffer, &Dirt_DebugSession, fd))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize FdWriteBuffer");
    exit(1);
   }
  if (!Dirt_StandardWriter_init(&writer, &callbacks, buffer))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize Writer");
    printf("Unable to initialize Writer");
    exit(1);
   }
  if (!writer.type->structure_list(&writer, NULL))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to write data");
    exit(1);
   }
  buffer->type->free(buffer);
  exit(0);
 }
