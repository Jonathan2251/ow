#ifndef TILIB_TVMONCMD_H_HEADER_INCLUDED_B8EA3E30
#define TILIB_TVMONCMD_H_HEADER_INCLUDED_B8EA3E30

#include "ModelConfig.h"
#include "BltcBasicType.h"

//##ModelId=471584A302B2
class TiLib_TvMonCmd
{
  public:
    //##ModelId=471584A302E1
    void RunCmdsInMenu(char* cmd[], S16 (* menu)( S8 * pcParm1, BOOL bHelpQuery ));

    //##ModelId=471584A302F0
    void RunCmdsNInMenu(char* cmd[], U16 size, S16 (* menu)( S8 * pcParm1, BOOL bHelpQuery ));

    //##ModelId=471584A30301
    void RunCmds(char* cmd[]);

    //##ModelId=471584A30303
    void RunCmdsN(char* cmd[], U16 size);
};



#endif /* TILIB_TVMONCMD_H_HEADER_INCLUDED_B8EA3E30 */
