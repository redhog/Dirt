#include <Dirt/Struct.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
 {
  Dirt_Struct *strct[2];
  Dirt_Struct *strctarr[3];
  char *namearr[3] = {"xxx", "yyy", "zzz"};
  Dirt_Struct **item;
  char **name;
  size_t items;
  Dirt_StructType *type;

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
 
  if (!(   (strctarr[0] = Dirt_Struct_str(&Dirt_DebugSession, "one", sizeof("one") - 1))
        && (strctarr[1] = Dirt_Struct_str(&Dirt_DebugSession, "two", sizeof("two") - 1))
        && (strctarr[2] = Dirt_Struct_str(&Dirt_DebugSession, "three", sizeof("three") - 1))))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Memory", "Unable to allocate Str:s to add to structure");
    exit(1);
   }

  if (!(strct[0] = Dirt_Struct_structure_add_array(&Dirt_DebugSession, strct[0], strctarr, 3)))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Memory", "Unable to add array to structure");
    exit(1);
   }

  if (!Dirt_Struct_structure_to_array(&Dirt_DebugSession, strct[0], &item, &items))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Memory", "Unable to convert structure to array");
    exit(1);
   }
  for (;items;items--)
   item[items-1]->type->decref(&Dirt_DebugSession, item[items-1]);

  if (!(   (strctarr[0] = Dirt_Struct_str(&Dirt_DebugSession, "one", sizeof("one") - 1))
        && (strctarr[1] = Dirt_Struct_str(&Dirt_DebugSession, "two", sizeof("two") - 1))
        && (strctarr[2] = Dirt_Struct_str(&Dirt_DebugSession, "three", sizeof("three") - 1))))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Memory", "Unable to allocate Str:s to add to structure");
    exit(1);
   }

  strct[0]->type->decref(&Dirt_DebugSession, strct[0]);


  if (!(strct[0] = Dirt_Struct_structure(&Dirt_DebugSession)))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Memory", "Unable to allocate Structure");
    exit(1);
   }

  if (!(strct[0] = Dirt_Struct_structure_add_pairs(&Dirt_DebugSession, strct[0], &Dirt_StructType_Keyvalue, namearr, strctarr, 3)))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Memory", "Unable to add pairs to structure");
    exit(1);
   }

  if (!(strct[0] = Dirt_Struct_structure_finalize_dictionary(&Dirt_DebugSession, strct[0])))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Memory", "Unable to finalize dictionary");
    exit(1);
   }

  if (!Dirt_Struct_structure_to_pairs(&Dirt_DebugSession, strct[0], &type, &name, &item, &items))
   {
    Dirt_DebugSession.type->error(&Dirt_DebugSession, "Memory", "Unable to convert structure to pairs");
    exit(1);
   }
  for (;items;items--)
   {
    item[items-1]->type->decref(&Dirt_DebugSession, item[items-1]);
    free(name[items-1]);
   }
  free(item);
  free(name);

  strct[0]->type->decref(&Dirt_DebugSession, strct[0]);

  exit(0);
 }
