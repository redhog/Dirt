#include <Dirt/Buffer.h>
#ifndef DIRT_READER
#define DIRT_READER

typedef struct Dirt_ReaderT Dirt_Reader;

typedef void     (*Dirt_Reader_Free)(Dirt_Reader *reader, void *item);
typedef void    *(*Dirt_Reader_Str)(Dirt_Reader *reader, char *str, size_t len);
typedef void    *(*Dirt_Reader_UnicodeStr)(Dirt_Reader *reader, char *str, size_t len);
typedef void    *(*Dirt_Reader_Identifier)(Dirt_Reader *reader, char *identifier, size_t len); 
typedef void    *(*Dirt_Reader_Num_Float)(Dirt_Reader *reader, float real);
typedef void    *(*Dirt_Reader_Num_Long)(Dirt_Reader *reader, long integer); 
typedef void    *(*Dirt_Reader_Num_Int)(Dirt_Reader *reader, int integer);
typedef void    *(*Dirt_Reader_None)(Dirt_Reader *reader);
typedef void    *(*Dirt_Reader_False)(Dirt_Reader *reader);
typedef void    *(*Dirt_Reader_True)(Dirt_Reader *reader);
typedef void    *(*Dirt_Reader_Structure)(Dirt_Reader *reader);
typedef void    *(*Dirt_Reader_Structure_Add)(Dirt_Reader *reader, void *structure, void *item);
typedef void    *(*Dirt_Reader_Structure_Finalize_Tuple)(Dirt_Reader *reader, void *structure);
typedef void    *(*Dirt_Reader_Structure_Finalize_List)(Dirt_Reader *reader, void *structure);
typedef void    *(*Dirt_Reader_Structure_Finalize_Dictionary)(Dirt_Reader *reader, void *structure);
typedef void    *(*Dirt_Reader_Structure_Finalize_Type)(Dirt_Reader *reader, void *structure);
typedef void    *(*Dirt_Reader_Keyvalue)(Dirt_Reader *reader, void *key, void *value);
typedef void    *(*Dirt_Reader_Parameter)(Dirt_Reader *reader, void *name, void *value);
typedef void    *(*Dirt_Reader_Member)(Dirt_Reader *reader, void *name, void *value);
typedef void    *(*Dirt_Reader_Application)(Dirt_Reader *reader, void *function, void *parameters);

typedef struct {
 Dirt_Reader_Free                             free;
 Dirt_Reader_Str                              str;
 Dirt_Reader_UnicodeStr                       unicodeStr;
 Dirt_Reader_Identifier                       identifier;
 Dirt_Reader_Num_Float                        num_float;
 Dirt_Reader_Num_Int                          num_int;
 Dirt_Reader_Num_Long                         num_long;
 Dirt_Reader_None                             none;
 Dirt_Reader_False                            false;
 Dirt_Reader_True                             true;
 Dirt_Reader_Structure                        structure;
 Dirt_Reader_Structure_Add                    structure_add;
 Dirt_Reader_Structure_Finalize_Tuple         structure_finalize_tuple;
 Dirt_Reader_Structure_Finalize_List          structure_finalize_list;
 Dirt_Reader_Structure_Finalize_Dictionary    structure_finalize_dictionary;
 Dirt_Reader_Structure_Finalize_Type          structure_finalize_type;
 Dirt_Reader_Keyvalue                         keyvalue;
 Dirt_Reader_Parameter                        parameter;
 Dirt_Reader_Member                           member;
 Dirt_Reader_Application                      application;
} Dirt_Reader_Callbacks;

typedef ssize_t  (*Dirt_Reader_ReadStr_ReadUnicodeescape)(Dirt_Reader *reader, off_t *pos, char *dst);
typedef ssize_t  (*Dirt_Reader_ReadStr_Read)(Dirt_Reader *reader, char *start, char startlen, char *dst);
typedef void    *(*Dirt_Reader_ReadStr)(Dirt_Reader *reader, char unicode);
typedef void    *(*Dirt_Reader_ReadNum)(Dirt_Reader *reader);
typedef ssize_t  (*Dirt_Reader_ReadIdentifier_Read)(Dirt_Reader *reader, char *dst);
typedef void    *(*Dirt_Reader_ReadIdentifier)(Dirt_Reader *reader);
typedef void    *(*Dirt_Reader_ReadIdentifiers)(Dirt_Reader *reader);
typedef void    *(*Dirt_Reader_ReadUnicodeOrId)(Dirt_Reader *reader);
typedef void    *(*Dirt_Reader_Read)(Dirt_Reader *reader, char inParens);

typedef struct {
 Dirt_Reader_ReadStr_ReadUnicodeescape    readStr_readUnicodeescape;
 Dirt_Reader_ReadStr_Read                 readStr_read;
 Dirt_Reader_ReadStr                      readStr;
 Dirt_Reader_ReadNum                      readNum;
 Dirt_Reader_ReadIdentifier_Read          readIdentifier_read;
 Dirt_Reader_ReadIdentifier               readIdentifier;
 Dirt_Reader_ReadIdentifiers              readIdentifiers;
 Dirt_Reader_ReadUnicodeOrId              readUnicodeOrId;
 Dirt_Reader_Read                         read;
} Dirt_ReaderType;

struct Dirt_ReaderT {
 Dirt_ReaderType       *type;
 Dirt_Reader_Callbacks *callback;
 Dirt_Buffer           *buffer;
};

char Dirt_StandardReader_init(Dirt_Reader *reader, Dirt_Reader_Callbacks *callbacks, Dirt_Buffer *buffer);

#endif

