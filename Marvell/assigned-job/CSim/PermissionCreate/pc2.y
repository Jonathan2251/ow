/* BNF for execute file pc (permission create):
example : refer to input file  "tmp.txt" */

%{
#include "c_interface.h"
%}

%start input

%token NUM STR ID DEFREG DEFTRAP DEFEND

%% /* Grammar rules and actions follow */

regname:
    ID
  | ID "_" exceptionlevel
;

exceptionlevel:
    ID
;

op0:
    NUM
;

defreg:
    DEFREG "(" regname "," exceptionlevel "," op0 ")"
    {
      strcpy(buff_op0, $7);
      CodeGen_CheckPermission_FuncName($1, $3, outcpp, outh);
    }
;

deftrap:
    DEFTRAP "(" STR "," exceptionlevel ")"
    {
      CodeGen_CheckPermission_ConditionTrap($3, $5, buff_op0, outcpp);
    }
;

deftraps:
    deftrap
  | deftraps deftrap
;

defend:
    DEFEND
    {
      CodeGen_CheckPermission_FunctionEnd(outcpp);
    }
;

defregperm:
    defreg deftraps defend
;

defregperms:
    defregperm
  | defregperms defregperm
;

input:
    defregperms
;

%%/* A lexical scanner generated by flex */
