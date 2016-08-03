TypException CheckPermission_define_regCPTR(TypSysRegRW /*RW*/, TypExceptionLevel level, TypContext& context)
{
  if ((EL2 == level || EL1 == level)&& 1 == QueryRegField(CPTR, EL3, nsFIELD::TCPAC) && !IsSecureAccess())
  {
    return Trap(EL3, context, 3);
  }

  return NO_TRAP;
}

TypException CheckPermission_define_regCPTT(TypSysRegRW /*RW*/, TypExceptionLevel level, TypContext& context)
{
  if ((EL2 == level) && 1 == QueryRegField(CPTR, EL3, nsFIELD::TCPAC) && IsSecureAccess() && 1 == QueryRegField(CPTR, EL2, nsFIELD::TCPAC))
  {
    return Trap(EL2, context, 3);
  }
  if ((EL1 == level) && 1 == QueryRegField(CPTR, EL3, nsFIELD::TCPAC) && !IsSecureAccess())
  {
    return Trap(EL3, context, 3);
  }

  return NO_TRAP;
}

