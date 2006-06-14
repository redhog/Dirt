#include <Dirt/WriteBuffer.h>

#define NEWREADER_WRITE_BUFFER_MAXSIZE 8192

void Dirt_WriteBuffer_free(Dirt_WriteBuffer *buffer);
char Dirt_WriteBuffer_adjust(Dirt_WriteBuffer *buffer);
char Dirt_WriteBuffer_advance(Dirt_WriteBuffer *buffer, size_t nr);
char Dirt_WriteBuffer_extend(Dirt_WriteBuffer *buffer, size_t nr);

char Dirt_WriteBuffer_init(Dirt_WriteBuffer *buffer, Dirt_WriteBufferType *type, Dirt_Session *session);
