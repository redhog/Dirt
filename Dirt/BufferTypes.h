#include <Dirt/Buffer.h>

char Dirt_StringBuffer_init(Dirt_Buffer *buffer, Dirt_Session *session, char *string);


struct Dirt_FdBufferT {
 Dirt_Buffer buffer;
 int fd;
};

typedef struct Dirt_FdBufferT Dirt_FdBuffer;

char Dirt_SocketBuffer_init(Dirt_FdBuffer *buffer, Dirt_Session *session, int fd);
char Dirt_FileBuffer_init(Dirt_FdBuffer *buffer, Dirt_Session *session, int fd);
