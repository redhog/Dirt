/* Buffers are defined as follows:

   ,---pos---.-------len--------.
   +---------+------------------+------------+
   |Garbage  | Data (lookahead) | Free space |
   +---------+------------------+------------+
	     `-----------size----------------'

   This allways holds for a buffer:

   pos < maxsize
   len <= size < len + maxsize

   We also _try_ to make the following hold as often as possible:
   
   len = maxsize

   So that the total buffer size is as near 3 * maxsize as
   possible. Note that this is only possible for some types of buffers
   (sockets).
*/

#ifndef DIRT_BUFFER_H
#define DIRT_BUFFER_H

#include <Dirt/Session.h>
#include <sys/types.h>

#define DIRT_BUFFER_NEW_MAXSIZE 8192

extern char Dirt_Buffer_Error_IO[];

typedef struct Dirt_BufferT Dirt_Buffer;
typedef struct Dirt_BufferTypeT Dirt_BufferType;

/* Call information:
   Functions marked with +buf should be called with the buffer locked,
   and ones marked with -buf should be called with the buffer lock
   released.
*/

typedef void    (*Dirt_Buffer_Free)(Dirt_Buffer *buffer); /* +buf */
typedef char    (*Dirt_Buffer_Advance)(Dirt_Buffer *buffer, size_t nr); /* +buf */
typedef char    (*Dirt_Buffer_Cut)(Dirt_Buffer *buffer, char *dst, size_t nr); /* +buf */
typedef char    (*Dirt_Buffer_Contains)(Dirt_Buffer *buffer, char *buf, size_t len, size_t at); /* +buf */
typedef ssize_t (*Dirt_Buffer_Read)(Dirt_Buffer *buffer, size_t nr); /* +buf */
typedef char    (*Dirt_Buffer_Extend)(Dirt_Buffer *buffer, size_t nr); /* +buf */

struct Dirt_BufferTypeT
 {
  Dirt_Session *session;

  Dirt_Buffer_Free free;

  Dirt_Buffer_Advance advance;
  Dirt_Buffer_Cut cut;
  Dirt_Buffer_Contains contains;
  Dirt_Buffer_Read read;
  Dirt_Buffer_Extend extend;
 };

struct Dirt_BufferT
 {
  Dirt_BufferType *type;
  Dirt_Session *session;
  char *buf;
  off_t pos;
  size_t len;
  size_t size;
  size_t maxsize;
 };

#endif
