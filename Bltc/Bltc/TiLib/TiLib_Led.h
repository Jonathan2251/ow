// Author : Gamma Chen

#ifndef TILIB_LED_H_HEADER_INCLUDED_B904B836
#define TILIB_LED_H_HEADER_INCLUDED_B904B836

#include "BltcBasicType.h"
#include "bled.h"

#define     SEGA     1       /* seg A */
#define	    SEGB     2       /* seg B */
#define     SEGC     4       /* seg C */
#define	    SEGD     8       /* seg D */
#define	    SEGE     16      /* seg E */ 
#define	    SEGF     32      /* seg F */
#define	    SEGG     64      /* seg G */
#define	    SEGDP    128     /* seg DP */

#define     ICON_REC1		0       // Left REC
#define     ICON_REC2		1       // Right REC
#define     ICON_720p		2       // Left REC
#define     ICON_480i		3       // Left REC
#define     ICON_R480p		4       // Left REC
#define     ICON_STANDBY	5       // Left REC
#define     ICON_1080i		6       // Left REC
#define     ICON_MESSAGE	7       // Left REC

#define MODEL_OTHER			0
#define MODEL_QIP7100_P2	1
#define MODEL_QIP72XX_P2	2

//##ModelId=46FA38C400CA
class TiLib_Led
{
  public:
    struct OnOffInfo {
    	U32 digit[4];
    	U32 accessory;
    };
    //##ModelId=4709F34603B4
  	TiLib_Led();
    //##ModelId=46FA38C40157
    virtual void Init();
    //##ModelId=48114D2702D8
    virtual void DisplayChar(char c, int pos);
    //##ModelId=46FA38C40169
    virtual void DisplayStr(char *str);
    //##ModelId=46FA38C40178
    virtual void AllOn();
    //##ModelId=46FA38C40179
    virtual void AllOff();
    //##ModelId=472F07AA00E9
    virtual void FastSpin(U32 nn);
    //##ModelId=472F0815002E
    virtual void SlowSpin();
    virtual OnOffInfo GetOnOffInfo();
    virtual void SetOnOff(OnOffInfo onOffInfo);

  protected:
    //##ModelId=46FA38C40149
    BLED_Handle _hLed;
};

class TiLib_Led_QIP7100_P2 : public TiLib_Led
{
  public:
    void AllOn();
    void AllOff();
    void FastSpin(U32 nn);
    void SlowSpin();
    OnOffInfo GetOnOffInfo();
    void SetOnOff(OnOffInfo onOffInfo);
};


class TiLib_Led_QIP72XX_P2 : public TiLib_Led
{
  public:
    void DisplayChar(char c, int pos);
    void DisplayStr(char *str);
    void AllOn();
    void AllOff();
    void FastSpin(U32 nn);
    void SlowSpin();
    OnOffInfo GetOnOffInfo();
    void SetOnOff(OnOffInfo onOffInfo);

  private:
    void Display_4Digits(U32 ulDisplay);
};


#endif /* TILIB_LED_H_HEADER_INCLUDED_B904B836 */
