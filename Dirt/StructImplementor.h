#ifndef DIRT_STRUCT_IMPLEMENTOR_H
#define DIRT_STRUCT_IMPLEMENTOR_H

#include <Dirt/Struct.h>

void Dirt_Struct_decref(Dirt_Session *session, Dirt_Struct *item);
Dirt_Struct *Dirt_Struct_incref(Dirt_Session *session, Dirt_Struct *item);

Dirt_Struct *Dirt_Struct_init(Dirt_Session *session, Dirt_StructType *type);

#endif
