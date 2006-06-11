#include "Buffer.h"

#define NEWREADER_BUFFER_MAXSIZE 8192

void Dirt_Buffer_free(Dirt_Buffer *buffer);
char Dirt_Buffer_advance(Dirt_Buffer *buffer, size_t nr) ;
char Dirt_Buffer_cut(Dirt_Buffer *buffer, char *dst, size_t nr);
char Dirt_Buffer_contains(Dirt_Buffer *buffer, char *buf, size_t len, size_t at);
char Dirt_Buffer_extend(Dirt_Buffer *buffer, size_t nr);

char Dirt_Buffer_init(Dirt_Buffer *buffer, Dirt_BufferType *type, Dirt_Session *session);
