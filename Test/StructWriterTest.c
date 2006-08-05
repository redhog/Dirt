#include <Dirt/Writer.h>
#include <Dirt/Struct.h>
#include <Dirt/StructWriter.h>
#include <Dirt/WriteBufferTypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

int main(int argc, char argv[])
 {
  Dirt_FdWriteBuffer fdbuffer;
  Dirt_WriteBuffer *buffer = (Dirt_WriteBuffer *) &fdbuffer;
  Dirt_Writer writer;
  Dirt_Struct *a, *b, *c;
  int fd;

  if ((fd = open("StructWriterTest.output", O_WRONLY | O_CREAT)) < 0)
   {
    perror("Unable to open file");
    exit(1);
   }
  if (!Dirt_FdWriteBuffer_init(&fdbuffer, &Dirt_DebugSession, fd))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize FdWriteBuffer");
    exit(1);
   }
  if (!Dirt_StandardWriter_init(&writer, &Dirt_StructWriter_Callbacks, buffer))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize Writer");
    printf("Unable to initialize Writer");
    exit(1);
   }

  a = Dirt_Struct_structure(&Dirt_DebugSession);
   a = Dirt_Struct_structure_add(&Dirt_DebugSession, a, Dirt_Struct_str(&Dirt_DebugSession, "foo", 3));
   a = Dirt_Struct_structure_add(&Dirt_DebugSession, a, Dirt_Struct_identifier(&Dirt_DebugSession, "foo", 3));
   b = Dirt_Struct_structure(&Dirt_DebugSession);
    b = Dirt_Struct_structure_add(&Dirt_DebugSession, b,
				  Dirt_Struct_keyvalue(&Dirt_DebugSession,
						       Dirt_Struct_str(&Dirt_DebugSession, "foo", 3),
						       Dirt_Struct_num_int(&Dirt_DebugSession, 4711)));
    b = Dirt_Struct_structure_add(&Dirt_DebugSession, b,
				  Dirt_Struct_keyvalue(&Dirt_DebugSession,
						       Dirt_Struct_str(&Dirt_DebugSession, "bar", 3),
						       Dirt_Struct_none(&Dirt_DebugSession)));
   b = Dirt_Struct_structure_finalize_dictionary(&Dirt_DebugSession, b);
   a = Dirt_Struct_structure_add(&Dirt_DebugSession, a, b);
  a = Dirt_Struct_structure_finalize_list(&Dirt_DebugSession, a);
  if (!writer.callback->write(&writer, a))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to write data");
    exit(1);
   }
  a->type->decref(&Dirt_DebugSession, a);
  buffer->type->free(buffer);
  exit(0);
 }
