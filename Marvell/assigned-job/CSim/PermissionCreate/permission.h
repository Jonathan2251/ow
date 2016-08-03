#ifndef __PERMISSION_H__
#define __PERMISSION_H__

#include "../execution/typedef.h"

namespace nsAArch64
{

struct TypContext;
typedef unsigned int TypSysRegIndex;

typedef enum
{
    NO_TRAP,
    TRAP_TO_EL1,
    TRAP_TO_EL2,
    TRAP_TO_EL3,
} TypException;

typedef enum
{
    SYSREG_READ,
    SYSREG_WRITE,
} TypSysRegRW;

TypException DefaultCheckPermission(TypSysRegRW RW, TypExceptionLevel level, TypContext& context);

#include "permission_codegen.h"

}

#endif
