// Author : Gamma Chen

#ifndef STATIONINFO_H_HEADER_INCLUDED_B9048FBE
#define STATIONINFO_H_HEADER_INCLUDED_B9048FBE

#include "Station.h"

//##ModelId=46FA16CC0291
struct StationInfo
{
    //##ModelId=46FA16CD008E
    S8 id;
    //##ModelId=46FA16CD009C
    U8 item[NR_ITEMS];
    //##ModelId=46FA16CD00AC
    U32 arg[NR_ARGLIST];
    //##ModelId=46FA16CD00AD
    U32 argSize;
};



#endif /* STATIONINFO_H_HEADER_INCLUDED_B9048FBE */
