// Author : Gamma Chen

#ifndef ITEM_H_HEADER_INCLUDED_B904D145
#define ITEM_H_HEADER_INCLUDED_B904D145

#include "BltcBasicType.h"
#include "ModelConfig.h"

#ifdef CONFIG_DISPLAY_STATION_INFO
//##ModelId=46FA16CC0264
struct Item
{
    //##ModelId=46FA16CC02A1
    U8 id;
    //##ModelId=46FA16CC037B
    S8 type;
    //##ModelId=46FA16CC038A
    S8	* name;
};
#else
struct Item {
	U8	id;
	S8	type;
};
#endif // CONFIG_DISPLAY_STATION_INFO



#endif /* ITEM_H_HEADER_INCLUDED_B904D145 */
