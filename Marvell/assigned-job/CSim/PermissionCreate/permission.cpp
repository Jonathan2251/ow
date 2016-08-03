#include "permission.h"
#include "system_register_manager.h"
#include "../execution/processor.h"
#include "../execution/exception_manager.h"
#include "../memory/memory.h"

namespace nsAArch64
{

bool IsSecureAccess(TypContext& context)
{
    return context.aarch64.pExceptionManager->IsSecure(context);
}

TypException Trap(TypExceptionLevel el, TypContext& context, int op0)
{
    TypWord opcode;
    context.aarch64.pMMU->DebugRead(context.aarch64.pProcessorState->ReadPC(), opcode);

    context.aarch64.pExceptionManager->SystemRegisterTrap(el, op0, opcode, context);

    if (el == EL1) return TRAP_TO_EL1;
    else if (el == EL2) return TRAP_TO_EL2;
    else if (el == EL3) return TRAP_TO_EL3;
    else return NO_TRAP;
}

TypUlong64 QueryRegField(TypSysRegIndex reg, TypExceptionLevel el, int field, TypContext& context)
{
    TypUlong64 value;

    context.aarch64.pSysRegManager->QueryReg(el, reg)->ReadByField(field, value);

    return value;
}

TypException DefaultCheckPermission(TypSysRegRW,  TypExceptionLevel,  TypContext&)
{
    return NO_TRAP;
}

TypException CheckPermission_CPTR_EL2(TypSysRegRW /*RW*/, TypExceptionLevel level, TypContext& context)
{
    if(EL2 == level && QueryRegField(CPTR, EL3, nsFIELD::TCPAC, context) && !IsSecureAccess(context))
    {
        return Trap(EL3, context, 3);
    }

    return NO_TRAP;
}

}

