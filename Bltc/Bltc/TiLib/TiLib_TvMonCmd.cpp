#include "TiLib_TvMonCmd.h"
#include "mainmenumgr.h"

//##ModelId=471584A302E1
void TiLib_TvMonCmd::RunCmdsInMenu(char* cmd[], S16 (* menu)( S8 * pcParm1, BOOL bHelpQuery ))
{
	tvMonIO_clear();
	tvMonIO_in_addCmds(cmd);
	menu("", FALSE);
}

//##ModelId=471584A302F0
void TiLib_TvMonCmd::RunCmdsNInMenu(char* cmd[], U16 size, S16 (* menu)( S8 * pcParm1, BOOL bHelpQuery ))
{
	tvMonIO_clear();
	tvMonIO_in_addCmdsN(cmd, size);
	menu("", FALSE);
}

#ifdef CONFIG_WITH_TVMON
//##ModelId=471584A30301
void TiLib_TvMonCmd::RunCmds(char* cmd[])
{
	tvMonIO_clear();
	tvMonIO_in_addCmds(cmd);
	CommandLineProcess();
}

//##ModelId=471584A30303
void TiLib_TvMonCmd::RunCmdsN(char* cmd[], U16 size)
{
	tvMonIO_clear();
	tvMonIO_in_addCmdsN(cmd, size);
	CommandLineProcess();
}
#endif





