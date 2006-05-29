#ifndef DIRT_SESSION_H
#define DIRT_SESSION_H

typedef struct Dirt_SessionT Dirt_Session;
typedef struct Dirt_SessionTypeT Dirt_SessionType;

typedef void (*Dirt_Session_Error)(Dirt_Session *session, char *class, char *error);

struct  Dirt_SessionTypeT
 {
  Dirt_Session_Error error;
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
