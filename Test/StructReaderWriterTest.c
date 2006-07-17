#include <Dirt/StructReader.h>
#include <Dirt/StructWriter.h>
#include <Dirt/Struct.h>
#include <Dirt/Reader.h>
#include <Dirt/Writer.h>
#include <Dirt/BufferTypes.h>
#include <Dirt/WriteBufferTypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdint.h>


int main(int argc, char argv[])
 {
  Dirt_FdBuffer infdbuffer;
  Dirt_Buffer *inbuffer = (Dirt_Buffer *) &infdbuffer;
  Dirt_Reader reader;
  int infd;

  Dirt_FdWriteBuffer outfdbuffer;
  Dirt_WriteBuffer *outbuffer = (Dirt_WriteBuffer *) &outfdbuffer;
  Dirt_Writer writer;
  int outfd;

  Dirt_Struct *strct;

  if ((infd = open("StructReaderWriterTest.data", O_RDONLY)) < 0)
   {
    perror("Unable to open input file");
    exit(1);
   }
  if (!Dirt_FileBuffer_init(&infdbuffer, &Dirt_DebugSession, infd))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize FileBuffer");
    exit(1);
   }
  if (!Dirt_StandardReader_init(&reader, &Dirt_StructReader_Callbacks, inbuffer))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize Reader");
    printf("Unable to initialize Reader");
    exit(1);
   }
  if ((outfd = open("StructReaderWriterTest.output", O_WRONLY | O_CREAT)) < 0)
   {
    perror("Unable to open outputfile");
    exit(1);
   }
  if (!Dirt_FdWriteBuffer_init(&outfdbuffer, &Dirt_DebugSession, outfd))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize FdWriteBuffer");
    exit(1);
   }
  if (!Dirt_StandardWriter_init(&writer, &Dirt_StructWriter_Callbacks, outbuffer))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize Writer");
    printf("Unable to initialize Writer");
    exit(1);
   }
  if (!(strct = (Dirt_Struct *) reader.type->read(&reader, 0)))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to read data");
    exit(1);
   }
  if (!writer.callback->write(&writer, strct))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to write data");
    exit(1);
   }
  if (!outbuffer->type->extend(outbuffer, 1))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to write newline at end of data (extend)");
    exit(1);
   }
  outbuffer->buf[outbuffer->pos] = '\n';
  if (!outbuffer->type->advance(outbuffer, 1))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to write newline at end of data (advance)");
    exit(1);
   }
  strct->type->free(&Dirt_DebugSession, strct);
  inbuffer->type->free(inbuffer);
  outbuffer->type->free(outbuffer);
  exit(0);
 }
