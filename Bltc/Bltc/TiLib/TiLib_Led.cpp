// Author : Gamma Chen

#include "TiLib_Led.h"
#include "BltcBasicType.h"
#include "fpcontrol.h"
#include "upg_handles.h"
#include "bchp_qam_3250.h"
#include "bchp_ldk.h"

#include "TiLib.h"

#define MAX_DISCRETES	4
extern "C" {
	unsigned char ucDiscreteArray[];		// only has 4 discretes for QIP7k
	unsigned char ucDigitArray[];
}

typedef struct BLED_P_Handle
{
	uint32_t		magicId;					/* Used to check if structure is corrupt */
	BCHP_Handle 	hChip;
	BREG_Handle		hRegister;
} BLED_P_Handle;

static unsigned char const LED_table[128] =
{
/* ASCII    */  /* LED Display */
/* 00-07    */  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* 08-0F    */  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* 10-17    */  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* 18-1F    */  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
/* 20 space */  0xFF,
/* 21 '!'   */  ~(SEGB+SEGC+SEGDP),
/* 22 HIo   */  ~(SEGA+SEGB+SEGF+SEGG),
/* 23       */  0xFF,
/* 24       */  0xFF,
/* 25       */  0xFF,
/* 26       */  0xFF,
/* 27 quote */  ~(SEGDP),
/* 28       */  0xFF,
/* 29       */  0xFF,
/* 2A       */  0xFF,
/* 2B       */  0xFF,
/* 2C       */  0xFF,
/* 2D '-'   */  ~(SEGG),
/* 2E '.'   */  ~(SEGDP),
/* 2F slash */  ~(SEGB+SEGE+SEGG),
/* 30 '0'   */  ~(SEGA+SEGB+SEGC+SEGD+SEGE+SEGF),
/* 31 '1'   */  ~(SEGB+SEGC),
/* 32 '2'   */  ~(SEGA+SEGB+SEGD+SEGE+SEGG),
/* 33 '3'   */  ~(SEGA+SEGB+SEGC+SEGD+SEGG),
/* 34 '4'   */  ~(SEGB+SEGC+SEGF+SEGG),
/* 35 '5'   */  ~(SEGA+SEGC+SEGD+SEGF+SEGG),
/* 36 '6'   */  ~(SEGA+SEGC+SEGD+SEGE+SEGF+SEGG),
/* 37 '7'   */  ~(SEGA+SEGB+SEGC),
/* 38 '8'   */  ~(SEGA+SEGB+SEGC+SEGD+SEGE+SEGF+SEGG),
/* 39 '9'   */  ~(SEGA+SEGB+SEGC+SEGD+SEGF+SEGG),
/* 3A ':'   */  0xFF,
/* 3B       */  0xFF,
/* 3C       */  0xFF,
/* 3D '='   */  ~(SEGA+SEGG),
/* 3E       */  0xFF,
/* 3F       */  0xFF,
/* 40       */  0xFF,
/* 41 'A'   */  ~(SEGA+SEGB+SEGC+SEGE+SEGF+SEGG),
/* 42 'B'   */  ~(SEGC+SEGD+SEGE+SEGF+SEGG),
/* 43 'C'   */  ~(SEGA+SEGD+SEGE+SEGF),
/* 44 'D'   */  ~(SEGB+SEGC+SEGD+SEGE+SEGG),
/* 45 'E'   */  ~(SEGA+SEGD+SEGE+SEGF+SEGG),
/* 46 'F'   */  ~(SEGA+SEGE+SEGF+SEGG),
/* 47 'G'   */  ~(SEGA+SEGC+SEGD+SEGE+SEGF+SEGG),
/* 48 'H'   */  ~(SEGB+SEGC+SEGE+SEGF+SEGG),
/* 49 'I'   */  ~(SEGB+SEGC),
/* 4A 'J'   */  ~(SEGB+SEGC+SEGD+SEGE),
/* 4B 'K'   */  ~(SEGB+SEGC+SEGE+SEGF+SEGG),
/* 4C 'L'   */  ~(SEGD+SEGE+SEGF),
/* 4D 'M'   */  ~(SEGA+SEGB+SEGC+SEGE+SEGF),
/* 4E 'N'   */  ~(SEGA+SEGB+SEGC+SEGE+SEGF),
/* 4F 'O'   */  ~(SEGA+SEGB+SEGC+SEGD+SEGE+SEGF),
/* 50 'P'   */  ~(SEGA+SEGB+SEGE+SEGF+SEGG),
/* 51 'Q'   */  ~(SEGA+SEGB+SEGC+SEGF+SEGG),
/* 52 'R'   */  ~(SEGA+SEGB+SEGC+SEGE+SEGF+SEGG),
/* 53 'S'   */  ~(SEGA+SEGC+SEGD+SEGF+SEGG),
/* 54 'T'   */  ~(SEGD+SEGE+SEGF+SEGG),
/* 55 'U'   */  ~(SEGB+SEGC+SEGD+SEGE+SEGF),
/* 56 'V'   */  ~(SEGC+SEGD+SEGE+SEGF),
/* 57 'W'   */  ~(SEGD+SEGE+SEGF),
/* 58 'X'   */  ~(SEGB+SEGC+SEGD),
/* 59 'Y'   */  ~(SEGB+SEGC+SEGD+SEGF+SEGG),
/* 5A 'Z'   */  ~(SEGA+SEGB+SEGD+SEGE+SEGG),
/* 5B '['   */  ~(SEGA+SEGD+SEGE+SEGF),
/* 5C '\'   */  ~(SEGC+SEGF+SEGG),
/* 5D ']'   */  ~(SEGA+SEGB+SEGC+SEGD),
/* 5E HIn   */  ~(SEGA+SEGB+SEGF),
/* 5F '_'   */  ~(SEGD),
/* 60       */  0xFF,
/* 61 'a'   */  ~(SEGA+SEGB+SEGC+SEGE+SEGF+SEGG),
/* 62 'b'   */  ~(SEGC+SEGD+SEGE+SEGF+SEGG),
/* 63 'c'   */  ~(SEGD+SEGE+SEGG),
/* 64 'd'   */  ~(SEGB+SEGC+SEGD+SEGE+SEGG),
/* 65 'e'   */  ~(SEGA+SEGB+SEGD+SEGE+SEGF+SEGG),
/* 66 'f'   */  ~(SEGA+SEGE+SEGF+SEGG),
/* 67 'g'   */  ~(SEGA+SEGB+SEGC+SEGD+SEGF+SEGG),
/* 68 'h'   */  ~(SEGC+SEGE+SEGF+SEGG),
/* 69 'i'   */  ~(SEGC),
/* 6A 'j'   */  ~(SEGB+SEGC+SEGD+SEGE),
/* 6B 'k'   */  ~(SEGB+SEGC+SEGE+SEGF+SEGG),
/* 6C 'l'   */  ~(SEGB+SEGC),
/* 6D 'm'   */  ~(SEGA+SEGD+SEGE+SEGF+SEGG),
/* 6E 'n'   */  ~(SEGC+SEGE+SEGG),
/* 6F 'o'   */  ~(SEGC+SEGD+SEGE+SEGG),
/* 70 'p'   */  ~(SEGA+SEGB+SEGE+SEGF+SEGG),
/* 71 'q'   */  ~(SEGA+SEGB+SEGC+SEGF+SEGG),
/* 72 'r'   */  ~(SEGE+SEGG),
/* 73 's'   */  ~(SEGA+SEGC+SEGD+SEGF+SEGG),
/* 74 't'   */  ~(SEGD+SEGE+SEGF+SEGG),
/* 75 'u'   */  ~(SEGC+SEGD+SEGE),
/* 76 'v'   */  ~(SEGC+SEGD+SEGE),
/* 77 'w'   */  ~(SEGE+SEGF+SEGG),
/* 78 'x'   */  ~(SEGB+SEGC+SEGG),
/* 79 'y'   */  ~(SEGB+SEGC+SEGD+SEGF+SEGG),
/* 7A 'z'   */  ~(SEGA+SEGE+SEGF),
/* 7B '{'   */  ~(SEGA+SEGD+SEGE+SEGF+SEGG),
/* 7C '|'   */  ~(SEGE+SEGF),
/* 7D '}'   */  ~(SEGA+SEGB+SEGC+SEGD+SEGG),
/* 7E '~'   */  ~(SEGA),
/* 7F       */  0xFF
};

//##ModelId=4709F34603B4
TiLib_Led::TiLib_Led()
{
}

//##ModelId=46FA38C40157
void TiLib_Led::Init()
{
    _hLed = bcmGetLedHandle();
}

//##ModelId=48114D2702D8
void TiLib_Led::DisplayChar(char c, int pos)
{
}

//##ModelId=46FA38C40169
void TiLib_Led::DisplayStr(char *str)
{
}

//##ModelId=46FA38C40178
void TiLib_Led::AllOn()
{
}

//##ModelId=46FA38C40179
void TiLib_Led::AllOff()
{
}


//##ModelId=472F07AA00E9
void TiLib_Led::FastSpin(U32 nn)
{
}

//##ModelId=472F0815002E
void TiLib_Led::SlowSpin(void)
{
}

TiLib_Led::OnOffInfo TiLib_Led::GetOnOffInfo()
{
	static  TiLib_Led::OnOffInfo onOffInfo;
	
	return onOffInfo;
}

void TiLib_Led::SetOnOff(TiLib_Led::OnOffInfo onOffInfo)
{
}

void TiLib_Led_QIP7100_P2::AllOn()
{
//	Light on all accessory leds
    BLED_SetDiscreteLED( _hLed, true, 0 );
    BLED_SetDiscreteLED( _hLed, true, 1 );
    BLED_SetDiscreteLED( _hLed, true, 2 );
}

void TiLib_Led_QIP7100_P2::AllOff()
{
//	Light off all accessory leds
    BLED_SetDiscreteLED( _hLed, false, 0 );
    BLED_SetDiscreteLED( _hLed, false, 1 );
    BLED_SetDiscreteLED( _hLed, false, 2 );
}


void TiLib_Led_QIP7100_P2::FastSpin(U32 nn)
{
    static unsigned long s_ul = 0;
    BLED_Handle          hLed = bcmGetLedHandle();
    long                 ii;
    long                 lOn = 0;
    
// gamma : copy from TvMonIO/bCmdLed.c and modify for loop times from 200 to nn.
//    for( ii = 0; ii < 200; ii++ )
    for( ii = 0; ii < nn; ii++ )
    {
        s_ul %= 8;
        s_ul++;
        
        if( s_ul == 8 )
        {
            lOn ^= 1;
            
            if( lOn )
            {
                BLED_SetDiscreteLED( hLed, true, 0 );
                BLED_SetDiscreteLED( hLed, true, 1 );
                BLED_SetDiscreteLED( hLed, true, 2 );
            }
            else
            {
                BLED_SetDiscreteLED( hLed, false, 0 );
                BLED_SetDiscreteLED( hLed, false, 1 );
                BLED_SetDiscreteLED( hLed, false, 2 );
            }

        }
        
        BKNI_Sleep( 30 );        
    }
}

void TiLib_Led_QIP7100_P2::SlowSpin(void)
{
    BLED_Handle   hLed = bcmGetLedHandle();
    unsigned char ucSegment;
    
	// Spin the accessory leds
	
   	for( ucSegment=0; ucSegment<=2; ucSegment++ )
	{
		BLED_SetDiscreteLED( hLed, true, ucSegment );		
 
		BKNI_Sleep( 250 );        

 		BLED_SetDiscreteLED( hLed, false, ucSegment );
         
        BKNI_Sleep( 250 );        
    }
}

 TiLib_Led::OnOffInfo TiLib_Led_QIP7100_P2::GetOnOffInfo()
{
    BLED_Handle   hLed = bcmGetLedHandle();
	static  TiLib_Led::OnOffInfo onOffInfo;
		
	onOffInfo.accessory = BREG_Read32 (hLed->hRegister, BCHP_LDK_STATUS);
	
	return onOffInfo;
}

void TiLib_Led_QIP7100_P2::SetOnOff( TiLib_Led::OnOffInfo onOffInfo)
{
    BLED_Handle   hLed = bcmGetLedHandle();
    	
	BREG_Write32 (hLed->hRegister, BCHP_LDK_STATUS, onOffInfo.accessory);
}

void TiLib_Led_QIP72XX_P2::Display_4Digits(U32 ulDisplay)
{
    volatile UCHAR ucOut;   //shifted value to display 

    ucOut = (UCHAR) ((ulDisplay & 0xFF000000) >> 24);
    BLED_Write( _hLed, 1, ucOut );
    ucOut = (UCHAR) ((ulDisplay & 0xFF0000) >> 16);
    BLED_Write( _hLed, 2, ucOut );
    
// Hardware swap digit 3 and 4    
    ucOut = (UCHAR) ((ulDisplay & 0xFF00) >> 8);
    BLED_Write( _hLed, 4, ucOut );
    ucOut = (UCHAR) ((ulDisplay & 0xFF));
    BLED_Write( _hLed, 3, ucOut );
}

void TiLib_Led_QIP72XX_P2::DisplayChar(char c, int pos)
{
	U8 uC;
	
	uC = LED_table[c];
	switch (pos)
	{
	case 0:
    	BLED_Write( _hLed, 1, uC );
    	break;
	case 1:
    	BLED_Write( _hLed, 2, uC );
    	break;
// Hardware swap digit 3 and 4
	case 2:
    	BLED_Write( _hLed, 4, uC );
    	break;
	case 3:
    	BLED_Write( _hLed, 3, uC );
    	break;
	}
}

void TiLib_Led_QIP72XX_P2::DisplayStr(char *str)
{
//	FrontPanelDisplay(pLEDstrg);

/*7-segments configuration
		  A
		-----
	   F|	| B
		| G	|
		-----
		|	| C
	   E|	|
		-----	. H
		  D
				HGFEDCBA
	BIT ORDER	7	   0 ACTIVE LOW
	
	if'1'= 0xF9;
	  '2'= 0xA4;
	  '3'= 0xB0;
	  '4'= 0x99;....etc 
*/

	unsigned i;
	U32 ulDisplay=0xffffffff;
	unsigned char *ptr;
	char s[5];

	if (strlen(str) < 4) {
		memset(s, ' ', 4);
		s[4] = '\0';
		memcpy(s, str, strlen(str));
	}
	else {
		s[4] = '\0';
		memcpy(s, str, 4);
	}
	for (i = 0; i < 4 && s[i]; i++) {
		ulDisplay=(ulDisplay<<8)|LED_table[s[i]];
	}
	ptr=(unsigned char*)((U32)&ulDisplay);

	Display_4Digits(ulDisplay);
}

void TiLib_Led_QIP72XX_P2::AllOn()
{
//	FrontPanelDisplay("8888");
    
    BLED_Write( _hLed, 1, 0 );
    BLED_Write( _hLed, 2, 0 );
    BLED_Write( _hLed, 3, 0 );
    BLED_Write( _hLed, 4, 0 );
    
//	Light on all accessory leds
    BLED_SetDiscreteLED( _hLed, true, 0 );
    BLED_SetDiscreteLED( _hLed, true, 1 );
    BLED_SetDiscreteLED( _hLed, true, 2 );
    BLED_SetDiscreteLED( _hLed, true, 3 );
}

void TiLib_Led_QIP72XX_P2::AllOff()
{
//	FrontPanelDisplay("    ");
    
    BLED_Write( _hLed, 1, 0xFF );
    BLED_Write( _hLed, 2, 0xFF );
    BLED_Write( _hLed, 3, 0xFF );
    BLED_Write( _hLed, 4, 0xFF );
    
//	Light off all accessory leds
    BLED_SetDiscreteLED( _hLed, false, 0 );
    BLED_SetDiscreteLED( _hLed, false, 1 );
    BLED_SetDiscreteLED( _hLed, false, 2 );
    BLED_SetDiscreteLED( _hLed, false, 3 );
}


void TiLib_Led_QIP72XX_P2::FastSpin(U32 nn)
{
    static unsigned long s_ul = 0;
    static unsigned char ucLedValue;
    BLED_Handle          hLed = bcmGetLedHandle();
    long                 ii;
    long                 lOn = 0;
    
// gamma : copy from TvMonIO/bCmdLed.c and modify for loop times from 200 to x.
//    for( ii = 0; ii < 200; ii++ )
    for( ii = 0; ii < nn; ii++ )
    {
        s_ul %= 8;
        switch( s_ul++ )
        {
            case 0: ucLedValue = 0xfe; break;
            case 1: ucLedValue = 0xfd; break;
            case 2: ucLedValue = 0xfb; break;
            case 3: ucLedValue = 0xf7; break;
            case 4: ucLedValue = 0xef; break;
            case 5: ucLedValue = 0xdf; break;
            case 6: ucLedValue = 0xbf; break;
            case 7: ucLedValue = 0xff; break;
        }

        BLED_Write( hLed, 1, ucLedValue );
        BLED_Write( hLed, 2, ucLedValue );
        BLED_Write( hLed, 3, ucLedValue );
        BLED_Write( hLed, 4, ucLedValue );
        
        if( s_ul == 8 )
        {
            lOn ^= 1;
            
            if( lOn )
            {
                BLED_Write( hLed, 1, ucLedValue & ~0x80 );
                BLED_Write( hLed, 2, ucLedValue & ~0x80 );
                BLED_Write( hLed, 3, ucLedValue & ~0x80 );
                BLED_Write( hLed, 4, ucLedValue & ~0x80 );
                BLED_SetDiscreteLED( hLed, true, 0 );
                BLED_SetDiscreteLED( hLed, true, 1 );
                BLED_SetDiscreteLED( hLed, true, 2 );
                BLED_SetDiscreteLED( hLed, true, 3 );
            }
            else
            {
                BLED_Write( hLed, 1, ucLedValue | 0x80 );
                BLED_Write( hLed, 2, ucLedValue | 0x80 );
                BLED_Write( hLed, 3, ucLedValue | 0x80 );
                BLED_Write( hLed, 4, ucLedValue | 0x80 );
                BLED_SetDiscreteLED( hLed, false, 0 );
                BLED_SetDiscreteLED( hLed, false, 1 );
                BLED_SetDiscreteLED( hLed, false, 2 );
                BLED_SetDiscreteLED( hLed, false, 3 );
            }

        }
        
        BKNI_Sleep( 30 );        
    }
}

// Follows copied from of Led_Spin() TvMonIO/bCmdLed.c.
void TiLib_Led_QIP72XX_P2::SlowSpin(void)
{
    unsigned char ucLedValue;
    BLED_Handle   hLed = bcmGetLedHandle();
    unsigned char ucDigit;
    unsigned char ucSegment;
    
    // Spin all the segments for each digit at a reasonable rate...
    
    for( ucDigit=0; ucDigit<MAX_DISCRETES; ucDigit++)
    {
       	for( ucSegment=0; ucSegment<=8; ucSegment++ )
   		{
       		// Exercise individual segments for each digit

        	switch( ucSegment )
        	{											// Segment
            	case 0: ucLedValue = 0xfe; break;		// a
            	case 1: ucLedValue = 0xfd; break;		// b
            	case 2: ucLedValue = 0xfb; break;		// c
            	case 3: ucLedValue = 0xf7; break;		// d
            	case 4: ucLedValue = 0xef; break;		// e
            	case 5: ucLedValue = 0xdf; break;		// f
            	case 6: ucLedValue = 0xbf; break;		// g
            	case 7: ucLedValue = 0xff; break;		// off
            	case 8: ucLedValue = 0x7F; break;		// DP on
        	}
		
			BLED_Write( hLed, ucDigitArray[ucDigit], ucLedValue ); 	// Digits 1 thru 4 valid
     		
			
			if( ucSegment==8 )		// Turn DP off
        	{	
     			BKNI_Sleep( 250 );
				
             	BLED_Write( hLed, ucDigitArray[ucDigit], ucLedValue | 0xFF );
            }
			
			BKNI_Sleep( 250 );        
		}
	}

	// Spin the accessory leds
	for( ucSegment=0; ucSegment<MAX_DISCRETES; ucSegment++ )
	{
		BLED_SetDiscreteLED( hLed, true, ucDiscreteArray[ucSegment] );		
 
		BKNI_Sleep( 400 );        

 		BLED_SetDiscreteLED( hLed, false, ucDiscreteArray[ucSegment] );
         
        BKNI_Sleep( 400 );        
    }
}

 TiLib_Led::OnOffInfo TiLib_Led_QIP72XX_P2::GetOnOffInfo()
{
    BLED_Handle   hLed = bcmGetLedHandle();
	static  TiLib_Led::OnOffInfo onOffInfo;
	
	onOffInfo.digit[0] = BREG_Read32 (hLed->hRegister, BCHP_LDK_DIGIT1);
	onOffInfo.digit[1] = BREG_Read32 (hLed->hRegister, BCHP_LDK_DIGIT2);
	onOffInfo.digit[2] = BREG_Read32 (hLed->hRegister, BCHP_LDK_DIGIT3);
	onOffInfo.digit[3] = BREG_Read32 (hLed->hRegister, BCHP_LDK_DIGIT4);
	
	onOffInfo.accessory = BREG_Read32 (hLed->hRegister, BCHP_LDK_STATUS);
	
	return onOffInfo;
}

void TiLib_Led_QIP72XX_P2::SetOnOff( TiLib_Led::OnOffInfo onOffInfo)
{
    BLED_Handle   hLed = bcmGetLedHandle();
    
	BREG_Write32 (hLed->hRegister, BCHP_LDK_DIGIT1, onOffInfo.digit[0]);
	BREG_Write32 (hLed->hRegister, BCHP_LDK_DIGIT2, onOffInfo.digit[1]);
	BREG_Write32 (hLed->hRegister, BCHP_LDK_DIGIT3, onOffInfo.digit[2]);
	BREG_Write32 (hLed->hRegister, BCHP_LDK_DIGIT4, onOffInfo.digit[3]);
	
	BREG_Write32 (hLed->hRegister, BCHP_LDK_STATUS, onOffInfo.accessory);
}
