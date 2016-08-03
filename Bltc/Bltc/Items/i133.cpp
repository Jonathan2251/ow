
#ifdef CONFIG_WITH_TUNER_TEST

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 Items::TI133()
{        
   api.tunerTest.PhaseNoiseTest(TUNER1);   
    	
   return (0);
}

#endif // #ifdef CONFIG_WITH_TUNER_TEST
