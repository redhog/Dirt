#ifndef DIRT_STRUCT_H
#define DIRT_STRUCT_H

#include <Dirt/Struct.h>

#define STR(s) Dirt_Struct_str(session, s, sizeof(s) - 1)
#define str(s) Dirt_Struct_str(session, s, strlen(s))
#define blob(s, l) Dirt_Struct_str(session, s, l)

#define USTR(s) Dirt_Struct_unicodeStr(session, s, sizeof(s) - 1)
#define ustr(s) Dirt_Struct_unicodeStr(session, s, strlen(s))
#define ublob(s, l) Dirt_Struct_unicodeStr(session, s, l)

#define IDENT(s) Dirt_Struct_identifier(session, s, sizeof(s) - 1)
#define ident(s) Dirt_Struct_identifier(session, s, strlen(s))
#define iblob(s, l) Dirt_Struct_identifier(session, s, l)

#define nfloat(f) Dirt_Struct_num_float(session, f)
#define nlong(i) Dirt_Struct_num_long(session, i)
#define nint(i) Dirt_Struct_num_int(session, i)

#define none() Dirt_Struct_none(session)
#define false() Dirt_Struct_false(session)
#define true() Dirt_Struct_true(session)

#define strct() Dirt_Struct_structure(session)
#define append(st, item) Dirt_Struct_structure_add(session, st, item)

#define tuple(st) Dirt_Struct_structure_finalize_tuple(session, st)
#define list(st) Dirt_Struct_structure_finalize_list(session, st)
#define dict(st) Dirt_Struct_structure_finalize_dictionary(session, st)
#define type(st) Dirt_Struct_structure_finalize_type(session, st)

#define keyval(k, v) Dirt_Struct_keyvalue(session, k, v)
#define param(n, v) Dirt_Struct_parameter(session, m, v)
#define member(n, v) Dirt_Struct_member(session, n, v)
#define apply(f, p) Dirt_Struct_application(session, f, p)

#endif
