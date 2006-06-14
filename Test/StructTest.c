#include <Dirt/Struct.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char argv[])
 {
  Dirt_Struct *strct[2];

  if (!(strct[0] = Dirt_Struct_structure(&Dirt_DebugSession)))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Memory", "Unable to allocate Structure");
    exit(1);
   }

  if (!(strct[1] = Dirt_Struct_str(&Dirt_DebugSession, "foobar", sizeof("foobar") - 1)))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Memory", "Unable to allocate Str");
    exit(1);
   }

  if (!(strct[0] = Dirt_Struct_structure_add(&Dirt_DebugSession, strct[0], strct[1])))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Memory", "Unable to add Str to structure");
    exit(1);
   }

  strct[0]->type->free(&Dirt_DebugSession, strct[0]);

  exit(0);
 }
