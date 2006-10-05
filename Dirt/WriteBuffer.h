/* WriteBuffers are defined as follows:

   ,---------------pos--------------.
   ,---wpos--.                      |
   +---------+----------------------+------------+
   |Garbage  | Data (to be written) | Free space |
   +---------+----------------------+------------+
	                             `---size----'

   This allways holds for a WriteBuffer:

   wpos <= pos <= 2 * maxsize
   wpos <= maxsize
   size < maxsize

   We also _try_ to make the following hold as often as possible:

   pos + size = 3 * maxsize

   So that the total WriteBuffer size is as near 2 * maxsize as
   possible.
*/

#ifndef DIRT_WRITE_BUFFER_H
#define DIRT_WRITE_BUFFER_H

#include <Dirt/Session.h>
#include <sys/types.h>

#define DIRT_WRITE_BUFFER_NEW_MAXSIZE 8192

extern char Dirt_WriteBuffer_Error_IO[];

typedef struct Dirt_WriteBufferT Dirt_WriteBuffer;
typedef struct Dirt_WriteBufferTypeT Dirt_WriteBufferType;

/* Call information:
   Functions marked with +buf should be called with the WriteBuffer locked,
   and ones marked with -buf should be called with the WriteBuffer lock
   freed.
*/

typedef void (*Dirt_WriteBuffer_Free)(Dirt_WriteBuffer *WriteBuffer); /* +buf */
typedef void (*Dirt_WriteBuffer_Release)(Dirt_WriteBuffer *WriteBuffer); /* +buf */
typedef void (*Dirt_WriteBuffer_Lock)(Dirt_WriteBuffer *WriteBuffer); /* -buf */
typedef char (*Dirt_WriteBuffer_Write)(Dirt_WriteBuffer *WriteBuffer, size_t nr); /* +buf */
typedef char (*Dirt_WriteBuffer_Adjust)(Dirt_WriteBuffer *buffer); /* +buf */
typedef char (*Dirt_WriteBuffer_Advance)(Dirt_WriteBuffer *buffer, size_t nr); /* +buf */
typedef char (*Dirt_WriteBuffer_Extend)(Dirt_WriteBuffer *WriteBuffer, size_t nr); /* +buf */

struct  Dirt_WriteBufferTypeT
 {
  Dirt_WriteBuffer_Free free;

  Dirt_WriteBuffer_Release release;
  Dirt_WriteBuffer_Lock lock;

  Dirt_WriteBuffer_Write write;
  Dirt_WriteBuffer_Adjust adjust;
  Dirt_WriteBuffer_Advance advance;
  Dirt_WriteBuffer_Extend extend;
 };

struct Dirt_WriteBufferT
 {
  Dirt_WriteBufferType *type;
  Dirt_Session *session;
  char *buf;
  off_t pos;
  size_t wpos;
  size_t size;
  size_t maxsize;
 };

#endif
