#include "WriteBuffer.h"

char Dirt_StringWriteBuffer_init(Dirt_WriteBuffer *buffer, Dirt_Session *session);


struct Dirt_FdWriteBufferT {
 Dirt_WriteBuffer buffer;
 int fd;
};

typedef struct Dirt_FdWriteBufferT Dirt_FdWriteBuffer;

char Dirt_FdWriteBuffer_init(Dirt_FdWriteBuffer *buffer, Dirt_Session *session, int fd);
