#ifndef DIRT_STRUCT_IMPLEMENTOR_H
#define DIRT_STRUCT_IMPLEMENTOR_H

#include <Dirt/Struct.h>

Dirt_Struct *Dirt_Struct_init(Dirt_Session *session, Dirt_StructType *type);
Dirt_Struct *Dirt_Struct_init_str(Dirt_Session *session, Dirt_StructType *type, char *str, size_t len);
Dirt_Struct *Dirt_Struct_keyvalue_init(Dirt_Session *session, Dirt_StructType *type, Dirt_Struct *key, Dirt_Struct *value);

#endif
