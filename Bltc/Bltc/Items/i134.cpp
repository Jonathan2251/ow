
#ifdef CONFIG_WITH_TUNER_TEST

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
                  
U32 Items::TI134()
{      
   api.tunerTest.PhaseNoiseTest(TUNER2);       	
   
   return (0);
}

#endif // #ifdef CONFIG_WITH_TUNER_TEST