#ifndef BLTC_H
#define BLTC_H

#include "ModelConfig.h"
#include "TiLib.h"
#include "board_id.h"

#define BLTC_VERSION_SHORT	"4014"
#define BLTC_VERSION_FULL 		CONFIG_STB_MODEL "_BLTC_V" BLTC_VERSION_SHORT "_TvMon12.23_2011-04-21"

extern int bltc_main();
extern TiLib lib;

#endif	// BLTC_H
