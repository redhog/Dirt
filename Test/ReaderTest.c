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

int level = 0;
#define LEAD() do { int i; for (i = level; i > 0; i--) printf(" "); } while(0)

size_t ReaderTest_unicodeLookupName(Dirt_Reader *reader, char *name, char *dst)       { LEAD(); printf("UnicodeLookupName: %s\n", name); return 0; };
size_t ReaderTest_unicodeLookupOrdinal(Dirt_Reader *reader, int ordinal, char *dst)   { LEAD(); printf("UnicodeLookupOrdinal: %i\n", ordinal); return 0; };
void   ReaderTest_free(Dirt_Reader *reader, void *item)                               { level--; LEAD(); printf("free: %p\n", item); };
void  *ReaderTest_str(Dirt_Reader *reader, char *str, size_t len)                     { LEAD(); printf("str: '%s'\n", str); return (void*)1; };
void  *ReaderTest_unicodeStr(Dirt_Reader *reader, char *str, size_t len)              { LEAD(); printf("unicodeStr: '%s'\n", str); return (void*)1; };
void  *ReaderTest_identifier(Dirt_Reader *reader, char *identifier, size_t len)       { LEAD(); printf("identifier: '%s'\n", identifier); return (void*)1; }; 
void  *ReaderTest_num_float(Dirt_Reader *reader, float real)                          { LEAD(); printf("float: %f\n", real); return (void*)1; };
void  *ReaderTest_num_long(Dirt_Reader *reader, long integer)                         { LEAD(); printf("long: %l\n", integer); return (void*)1; }; 
void  *ReaderTest_num_int(Dirt_Reader *reader, int integer)                           { LEAD(); printf("int: %i\n", integer); return (void*)1; };
void  *ReaderTest_none(Dirt_Reader *reader)                                           { LEAD(); printf("none\n"); return (void*)1; };
void  *ReaderTest_false(Dirt_Reader *reader)                                          { LEAD(); printf("false\n"); return (void*)1; };
void  *ReaderTest_true(Dirt_Reader *reader)                                           { LEAD(); printf("true\n"); return (void*)1; };
void  *ReaderTest_structure(Dirt_Reader *reader)                                      { LEAD(); level++; printf("structure\n"); return (void*)1; };
void  *ReaderTest_structure_add(Dirt_Reader *reader, void *structure, void *item)     { LEAD(); printf("structure_add\n"); return (void*)1; };
void  *ReaderTest_structure_finalize_tuple(Dirt_Reader *reader, void *structure)      { level--; LEAD(); printf("structure_finalize_tuple\n"); return (void*)1; };
void  *ReaderTest_structure_finalize_list(Dirt_Reader *reader, void *structure)       { level--; LEAD(); printf("structure_finalize_list\n"); return (void*)1; };
void  *ReaderTest_structure_finalize_dictionary(Dirt_Reader *reader, void *structure) { level--; LEAD(); printf("structure_finalize_dictionary\n"); return (void*)1; };
void  *ReaderTest_structure_finalize_type(Dirt_Reader *reader, void *structure)       { level--; LEAD(); printf("structure_finalize_type\n"); return (void*)1; };
void  *ReaderTest_keyvalue(Dirt_Reader *reader, void *key, void *value)               { LEAD(); printf("keyvalue\n"); return (void*)1; };
void  *ReaderTest_parameter(Dirt_Reader *reader, void *name, void *value)             { LEAD(); printf("parameter\n"); return (void*)1; };
void  *ReaderTest_member(Dirt_Reader *reader, void *name, void *value)                { LEAD(); printf("member\n"); return (void*)1; };
void  *ReaderTest_application(Dirt_Reader *reader, void *function, void *parameters)  { LEAD(); printf("application\n"); return (void*)1; };

Dirt_Reader_Callbacks callbacks = {
 ReaderTest_unicodeLookupName,
 ReaderTest_unicodeLookupOrdinal,
 ReaderTest_free,
 ReaderTest_str,
 ReaderTest_unicodeStr,
 ReaderTest_identifier,
 ReaderTest_num_float,
 ReaderTest_num_int,
 ReaderTest_num_long,
 ReaderTest_none,
 ReaderTest_false,
 ReaderTest_true,
 ReaderTest_structure,
 ReaderTest_structure_add,
 ReaderTest_structure_finalize_tuple,
 ReaderTest_structure_finalize_list,
 ReaderTest_structure_finalize_dictionary,
 ReaderTest_structure_finalize_type,
 ReaderTest_keyvalue,
 ReaderTest_parameter,
 ReaderTest_member,
 ReaderTest_application
};

int main(int argc, char argv[])
 {
  Dirt_FdBuffer fdbuffer;
  Dirt_Buffer *buffer = (Dirt_Buffer *) &fdbuffer;
  Dirt_Reader reader;
  int fd;

  if ((fd = open("ReaderTest.data", O_RDONLY)) < 0)
   {
    perror("Unable to open file");
    exit(1);
   }
  if (!Dirt_FileBuffer_init(&fdbuffer, &Dirt_DebugSession, fd))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize FileBuffer");
    exit(1);
   }
  if (!Dirt_StandardReader_init(&reader, &callbacks, buffer))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Init", "Unable to initialize Reader");
    printf("Unable to initialize Reader");
    exit(1);
   }
  if (!reader.type->read(&reader, 0))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "I/O error", "Unable to read data");
    exit(1);
   }
  exit(0);
 }
