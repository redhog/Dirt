#include "Session.h"
#include <stdio.h>

void Dirt_DebugSession_error(Dirt_Session *session, char *class, char *error)
 {
  fprintf(stderr, "DIRT ERROR: %s: %s\n", class, error);
  fflush(stderr);
 }

void Dirt_SilentSession_error(Dirt_Session *session, char *class, char *error)
 {
 }

Dirt_SessionType Dirt_DebugSessionType = { &Dirt_DebugSession_error };
Dirt_Session Dirt_DebugSession = { &Dirt_DebugSessionType };
Dirt_SessionType Dirt_SilentSessionType = { &Dirt_SilentSession_error };
Dirt_Session Dirt_SilentSession = { &Dirt_SilentSessionType };
