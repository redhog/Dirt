#include "StructReader.h"
#include "Struct.h"
#include "Reader.h"
#include "BufferTypes.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdint.h>

void printStruct(Dirt_Struct *strct)
 {
  size_t pos;
  if (strct->type == &Dirt_StructType_Str) printf("'''%*s'''", strct->str.len, strct->str.str);
  else if (strct->type == &Dirt_StructType_UnicodeStr) printf("u'''%*s'''", strct->str.len, strct->str.str);
  else if (strct->type == &Dirt_StructType_Identifier) printf("%*s", strct->str.len, strct->str.str);
  else if (strct->type == &Dirt_StructType_Num_Float) printf("%f", strct->num_float);
  else if (strct->type == &Dirt_StructType_Num_Long) printf("%l", strct->num_long);
  else if (strct->type == &Dirt_StructType_Num_Int) printf("%i", strct->num_int);
  else if (strct->type == &Dirt_StructType_None) printf("None");
  else if (strct->type == &Dirt_StructType_False) printf("False");
  else if (strct->type == &Dirt_StructType_True) printf("True");
  else if (strct->type == &Dirt_StructType_Structure)
   {
    printf("Structure(");
    for (pos = 0; pos < strct->structure.len; pos++)
     {
      printStruct(strct->structure.items[pos]);
      if (pos < strct->structure.len - 1)
       printf(", ");
     }
    printf(")");
   } 
  else if (strct->type == &Dirt_StructType_Structure_Tuple)
   {
    printf("(");
    for (pos = 0; pos < strct->structure.len; pos++)
     {
      printStruct(strct->structure.items[pos]);
      if (pos < strct->structure.len - 1)
       printf(", ");
     }
    printf(")");
   }
  else if (strct->type == &Dirt_StructType_Structure_List)
   {
    printf("[");
    for (pos = 0; pos < strct->structure.len; pos++)
     {
      printStruct(strct->structure.items[pos]);
      if (pos < strct->structure.len - 1)
       printf(", ");
     }
    printf("]");
   }
  else if (strct->type == &Dirt_StructType_Structure_Dictionary)
   {
    printf("{");
    for (pos = 0; pos < strct->structure.len; pos++)
     {
      printStruct(strct->structure.items[pos]);
      if (pos < strct->structure.len - 1)
       printf(", ");
     }
    printf("}");
   } 
  else if (strct->type == &Dirt_StructType_Structure_Type)
   {
    printf("<");
    for (pos = 0; pos < strct->structure.len; pos++)
     {
      printStruct(strct->structure.items[pos]);
      if (pos < strct->structure.len - 1)
       printf(", ");
     }
    printf(">");
   }
  else if (strct->type == &Dirt_StructType_Keyvalue)
   {
    printStruct(strct->keyvalue.key);
    printf(":");
    printStruct(strct->keyvalue.value);
   } 
  else if (strct->type == &Dirt_StructType_Parameter)
   {
    printStruct(strct->keyvalue.key);
    printf("=");
    printStruct(strct->keyvalue.value);
   } 
  else if (strct->type == &Dirt_StructType_Member)
   {
    printStruct(strct->keyvalue.key);
    printf(".");
    printStruct(strct->keyvalue.value);
   }    
  else if (strct->type == &Dirt_StructType_Application)
   {
    printStruct(strct->keyvalue.key);
    printf("(");
    for (pos = 0; pos < strct->keyvalue.value->structure.len; pos++)
     {
      printStruct(strct->keyvalue.value->structure.items[pos]);
      if (pos < strct->keyvalue.value->structure.len - 1)
       printf(", ");
     }
    printf(")");
  }
 }

int main(int argc, char argv[])
 {
  Dirt_Struct *strct;
  Dirt_FdBuffer fdbuffer;
  Dirt_Buffer *buffer = (Dirt_Buffer *) &fdbuffer;
  Dirt_Reader reader;
  int fd;

  if (!(fd = open("ReaderTest.data", O_RDONLY)))
   {
    perror("Unable to open file");
    exit(1);
   }
  if (!Dirt_FileBuffer_init(&fdbuffer, &Dirt_DebugSession, fd))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize FileBuffer");
    exit(1);
   }
  if (!Dirt_StandardReader_init(&reader, &Dirt_StructReader_Callbacks, buffer))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize Reader");
    printf("Unable to initialize Reader");
    exit(1);
   }
  if (!(strct = (Dirt_Struct *) reader.type->read(&reader, 0)))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to read data");
    exit(1);
   }

  printStruct(strct);
  printf("\n");

  strct->type->free(&Dirt_DebugSession, strct);

  exit(0);
 }
