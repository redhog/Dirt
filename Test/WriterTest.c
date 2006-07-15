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

void *WriterTest_structure_begin(Dirt_Writer *writer, void *structure) { return (void *) 1; };
char  WriterTest_structure_end(Dirt_Writer *writer, void *iter) { return listlen >= 10000; };
char  WriterTest_structure_writeNext(Dirt_Writer *writer, void *iter) { listlen *= 2; return writer->type->num_int(writer, listlen); };
char  WriterTest_keyvalue_writeKey(Dirt_Writer *writer, void *keyvalue) { return 0; };
char  WriterTest_keyvalue_writeValue(Dirt_Writer *writer, void *keyvalue) { return 0; };

Dirt_Writer_Callbacks callbacks = {
 WriterTest_structure_begin,
 WriterTest_structure_end,
 WriterTest_structure_writeNext,
 WriterTest_keyvalue_writeKey,
 WriterTest_keyvalue_writeValue
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
