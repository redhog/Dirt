#ifndef DIRT_SESSION_H
#define DIRT_SESSION_H
#include <sys/types.h>

typedef struct Dirt_SessionT Dirt_Session;
typedef struct Dirt_SessionTypeT Dirt_SessionType;

typedef void   (*Dirt_Session_Error)(Dirt_Session *session, char *class, char *error);
typedef size_t (*Dirt_Session_UnicodeLookupName)(Dirt_Session *session, char *name, char *dst);
typedef size_t (*Dirt_Session_UnicodeLookupOrdinal)(Dirt_Session *session, int ordinal, char *dst);

struct  Dirt_SessionTypeT
 {
  Dirt_Session_Error error;
  Dirt_Session_UnicodeLookupName unicodeLookupName;
  Dirt_Session_UnicodeLookupOrdinal unicodeLookupOrdinal;
 };

struct Dirt_SessionT
 {
  Dirt_SessionType *type;
 };

Dirt_SessionType Dirt_DebugSessionType;
Dirt_Session Dirt_DebugSession;
Dirt_SessionType Dirt_SilentSessionType;
Dirt_Session Dirt_SilentSession;

#endif
