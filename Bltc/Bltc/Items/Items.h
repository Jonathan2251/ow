#ifndef ITEMS_H_HEADER_INCLUDED_B7D44F75
#define ITEMS_H_HEADER_INCLUDED_B7D44F75
#include "Item.h"
#include "Process.h"
#include "ModelConfig.h"
#include "BltcBasicType.h"

#define NO_ITEM_TYPE		0
#define TEST_ITEM_TYPE		1
#define ACTION_ITEM_TYPE	2
#define CONTROL_ITEM_TYPE	3
#define ITEM_WAIT_KEYIN		254
#define ITEM_DELAY_100MS	253

enum {TI_SUCCESS = 0, TI_FAILURE, TI_NO_THIS_ITEM};

extern Item gItem[256];

//##ModelId=46CBAA8503E2
class Items {
  public:
    //##ModelId=46DFC02E03CB
    U32 TI000();
    //##ModelId=46FB3FDA02AC
    U32 TI001();
    //##ModelId=46EE3153037F
    U32 TI002();
    //##ModelId=46EE1FE20058
    U32 TI003();
    //##ModelId=480E9C4402C8
    void AI004();
    //##ModelId=46EE1FE801C0
    void AI005();
    //##ModelId=482B9CF50101
    U32 TI006();
    //##ModelId=47022D7D00BE
    U32 TI007();
    //##ModelId=47022D850225
    U32 TI008();
    //##ModelId=47022D8D0199
    U32 TI009();
    //##ModelId=46EE1CC50011
    U32 TI010();
    //##ModelId=46EE1D040346
    void AI011();
    //##ModelId=47022D6B011C
    void AI012();
    U32 TI013();
    U32 TI014();
    U32 TI015();
    U32 TI017();
    U32 TI020();
    //##ModelId=470DFF390006
    U32 TI021();
    //##ModelId=470DFF400073
    U32 TI022();
    //##ModelId=470DFF440303
    U32 TI023();
    //##ModelId=470DFF48020A
    U32 TI024();
    //##ModelId=470DFF4C02C5
    U32 TI025();
    //##ModelId=470DFF500248
    U32 TI026();
    //##ModelId=470DFF5400C2
    U32 TI027();
    //##ModelId=46F20A1A01CE
    U32 TI028();
    //##ModelId=46F20A210046
    U32 TI029();
    U32 TI030();
    //##ModelId=470DFFAB01AD
    U32 TI031();
    //##ModelId=46F20AB80008
    U32 TI032();
    //##ModelId=46F20ACD01F8
    U32 TI033();
    //##ModelId=46F20AD001D8
    U32 TI034();
    //##ModelId=46F20AD3011C
    U32 TI035();
    //##ModelId=46F20AD6032E
    U32 TI036();
    //##ModelId=46F20ADA036C
    U32 TI037();
    //##ModelId=470DFFC4022A
    U32 TI038();
    //##ModelId=46E8FE630146
    U32 TI039();
    //##ModelId=470DFFCF03B1
    U32 TI040();
    //##ModelId=470DFFD30373
    U32 TI041();
    //##ModelId=470DFFD80075
    U32 TI042();
    //##ModelId=470DFFDC018E
    U32 TI043();
    U32 TI044();
    U32 TI045();
    //##ModelId=482B97F9020C
    void AI051();
    //##ModelId=46DB803203E7
    U32 TI052();
    //##ModelId=470E007F0191
    U32 TI053();
    //##ModelId=470E008302E8
    U32 TI054();
    //##ModelId=470E00880327
    U32 TI055();
    //##ModelId=470E008D0308
    U32 TI056();
    //##ModelId=470E0092029A
    U32 TI057();
    U32 TI058();
    U32 TI059();
    U32 TI060();
    //##ModelId=470E00A70337
    U32 TI061();
    //##ModelId=470E00AB025C
    U32 TI062();
    //##ModelId=470E00AF02AA
    U32 TI063();
    //##ModelId=470E00B3023D
    U32 TI064();
    U32 TI065();
    U32 TI066();
    U32 TI067();
    //##ModelId=46F211D7000E
    U32 TI071();
    //##ModelId=46F211DD02CC
    U32 TI073();
    //##ModelId=46F211E001C2
    U32 TI074();
    U32 TI075();
    //##ModelId=46F20D0B013F
    U32 TI082();
    //##ModelId=46F20D2101DE
    U32 TI083();
    //##ModelId=46F20BBD0006
    U32 TI084();
    //##ModelId=46F20BC10010
    U32 TI085();
    //##ModelId=46F20BC7039D
    U32 TI087();
    //##ModelId=46CBAAB30376
    U32 TI088();
    //##ModelId=470E003F027A
    U32 TI089();
    //##ModelId=470E00440048
    U32 TI090();
    //##ModelId=470E00480326
    U32 TI091();
    //##ModelId=470E004C02B9
    U32 TI092();
    //##ModelId=470E00500345
    U32 TI093();
    U32 TI094();
    
#ifdef CONFIG_WITH_TUNER_TEST
    U32 TI133();
    U32 TI134();
    U32 TI135();
    U32 TI136();
#endif // #ifdef CONFIG_WITH_TUNER_TEST
    
    //##ModelId=4705AB760210
    void AI201();
    //##ModelId=4705AB890397
    void AI202();
#ifdef CONFIG_WITH_OSD
    //##ModelId=4705AB9102BC
    void AI203();
    void AI204();
#endif	// CONFIG_WITH_OSD
    //##ModelId=473302D103B1
    void AI205();
    //##ModelId=480E9C4700F3
    void AI206();
    //##ModelId=480E9C47024B
    void AI207();
    //##ModelId=480E9C470393
    void AI208();
    void AI209();
    void AI210();
#ifdef CONFIG_WITH_OSD
    void AI211();
#endif	// CONFIG_WITH_OSD
    void AI212();

    //##ModelId=473302EA0037
    void CI253(U32 arg);

  private:
#ifdef CONFIG_ENABLE_TIMER_INTERRUPT
    //##ModelId=4735722E03C8
    Process _process;
#endif
};

extern Items	items;


#endif /* ITEMS_H_HEADER_INCLUDED_B7D44F75 */
