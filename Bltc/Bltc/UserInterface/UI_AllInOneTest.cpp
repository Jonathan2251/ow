/*************************************************************************************************************
Copyright (c)   2007 General Instruments Corporation
Function:       Board Level All In One Test

Parameter:      None
Return:         None
Author:         Rogers Liu
Version:		v0.1
						
*************************************************************************************************************/
/********************************************************************/
/*                  Include Head File                               */
/********************************************************************/
#include "string.h"
#include "TiApi.h"
#include "TiLib.h"
#include "UI_AllInOneTest.h" 
#include "ModelConfig.h"
#include "crccalc.h"
#include "cputimer.h"
#include "userio.h"
#include "gpio.h"

/********************************************************************/
/*                  Define Macro &  Constants                       */
/********************************************************************/
//#define AIO_TX  lib.rs232.Print     //bcmDbgOutNoNewLine();
//#define AIO_RX  lib.rs232.RxByte    //GetcCharNoBlock(); 
 

/********************************************************************/
/*                  Define Public variable                          */
/********************************************************************/     
static TimeStruct  AIO_Timer;

  
/********************************************************************/
/*                  Call external variable                          */
/********************************************************************/
 
  
/********************************************************************/
/*                  Call external function                          */
/********************************************************************/ 
                       
/********************************************************************/
/*                  Call internal function                          */
/********************************************************************/ 
U32 ClearRS232Buffer(void);


/***************************************************************************************************/ 
/*-------------------------------------------------------------------------------------------------*/    
U32 ClearRS232Buffer(void)
{
   U8 DummyData=0;  
   
   Init_timer(&AIO_Timer,AIO_SERVICE_TIME_1s);      
   while(lib.rs232.RxByte(&DummyData)==SUCCESS)
   {
      if(Check_timer(&AIO_Timer)) return FAILURE;    
   }
   
   return SUCCESS;
}


//##ModelId=48102AEF01FD
UI_AllInOneTest::UI_AllInOneTest(UI_OutputMsg *outMsg) : UserInterface(outMsg)
{
}

int UI_AllInOneTest::AIO_TX(S8* pcFormat, ...)
{
	int result = 0;
	bool rs232enabled = lib.rs232.EnableOutput();
	va_list ap;
   
	va_start(ap, pcFormat);
	result = lib.rs232.Print_WithArg(pcFormat, ap);
	va_end(ap);    // clean things up before leaving
	
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
	
	return result;
}

U32 UI_AllInOneTest::AIO_RX(U8* rxByte)
{
	return lib.rs232.RxByte(rxByte);
}


/***************************************************************************************************/ 
/*-------------------------------------------------------------------------------------------------*/    
void UI_AllInOneTest::ClearTestItem(void)
{
   U8 i;
      
   for(i=0;i<AIO_Size_Maximum_TI;i++) TestItem_Queue[i]=0;
}  

void UI_AllInOneTest::TestItem_Debug(void)
{
   U8 i;
      
   AIO_TX("\n\rAIO_TestItem=");             
   for(i=0;i<AIO_Size_Maximum_TI;i++) AIO_TX("%.3d,",TestItem_Queue[i]);       
   AIO_TX("\n\r");  
}
    
/***************************************************************************************************/ 
/*-------------------------------------------------------------------------------------------------*/
void UI_AllInOneTest::ClearBuffer(void)
{
   U8 i;
      
   for(i=0;i<AIO_Size_Multiple_TI;i++) AIO_Buffer[i]='?';
}
  
void UI_AllInOneTest::Buffer_Debug(void)
{
   U8 i;
      
   AIO_TX("\n\rAIO_TestItem=");             
   for(i=0;i<AIO_Size_Multiple_TI;i++) AIO_TX("%c",AIO_Buffer[i]);       
   AIO_TX("\n\r");
}   

/***************************************************************************************************/ 
/*-------------------------------------------------------------------------------------------------*/
AIO_DF_ErrCode UI_AllInOneTest::Protocol_Filter(U32 TimeOut,S8 *Protocol_Content,U8 length)
{
   U8 i,TimeOutFlag;    
   U8 AIO_data=0,Result=FAILURE;         
   
   if(TimeOut!=AIO_SERVICE_TIME_HOLD) Init_timer(&AIO_Timer,TimeOut);   
      
   for(i=0;i<length;i++)
   {     
      if(TimeOut==AIO_SERVICE_TIME_HOLD) while(AIO_RX(&AIO_data)==FAILURE){}
      else
      {           
         while(AIO_RX(&AIO_data)==FAILURE)
         {
            if(TimeOutFlag=Check_timer(&AIO_Timer)) return AIO_DF_TimeOut;            
         }    
      }      
      
      if(AIO_data!=Protocol_Content[i]) i=0;
   }   
    
   return AIO_DF0_SUCCESS;  
}

/***************************************************************************************************/ 
/*-------------------------------------------------------------------------------------------------*/
AIO_DF_ErrCode UI_AllInOneTest::Protocol_Dual_Filter(U32 TimeOut,S8 *Protocol_Content_1,S8 *Protocol_Content_2,U8 length)
{
   U8 i,TimeOutFlag;
   U8 AIO_data=0,Result=FAILURE;
   U8 IdentifyTick[2]={0,0};
  
   if(TimeOut!=AIO_SERVICE_TIME_HOLD) Init_timer(&AIO_Timer,TimeOut);  
      
   for(i=0;i<length;i++)
   { 
      if(TimeOut==AIO_SERVICE_TIME_HOLD) while(AIO_RX(&AIO_data)==FAILURE){}
      else
      {           
         while(AIO_RX(&AIO_data)==FAILURE)
         {
            if(TimeOutFlag=Check_timer(&AIO_Timer)) return AIO_DF_TimeOut;            
         }    
      }         
     
      if( AIO_data==Protocol_Content_1[i]) IdentifyTick[0]++;
      if( AIO_data==Protocol_Content_2[i]) IdentifyTick[1]++;
      if((AIO_data!=Protocol_Content_1[i]) && (AIO_data!=Protocol_Content_2[i])) i=0;      
   }   
    
   if(IdentifyTick[0]==length) return AIO_DF1_SUCCESS;  
    
   return AIO_DF2_SUCCESS;     
}

/***************************************************************************************************/ 
/*-------------------------------------------------------------------------------------------------*/
void UI_AllInOneTest::HANDSHAKING(CHAR *ProductName)
{
   U8  length,i;  
   S8  H_String[20];  // for storing the string of the handshaking.
   U16 Checksum;
   
   /* To combine the HANDSHAKING code with the project name into a string */                  
   H_String[0]='H';
   H_String[1]='-';     
   i=0;
   length=2;            
   while(ProductName[i]!='\0') H_String[length++]= ProductName[i++];   
   H_String[length]='\0';       
   
   /* To get CRC16 value */   
   Checksum=CalcCRC16((U8*)H_String,length,0,0,0,0);
        
   AIO_TX("\r\n");
         
   AIO_HANDSHAKING_Dispatch:
	lib.peripheral.WatchDog_Serve();
   /***********************/         
   AIO_TX("%s%s%.4X%s\r\n\r",Leader_Code,H_String,Checksum,Stop_Code); /* Dispatching the handshaking to SIE */  
   
   AIO_HANDSHAKING_Re_filter:  
   /************************/              
   if(Protocol_Filter(AIO_SERVICE_TIME_1s,Leader_Code,Size_Leader_Code)==AIO_DF_TimeOut) goto AIO_HANDSHAKING_Dispatch;   
   if(Protocol_Filter(AIO_SERVICE_TIME_10ms,H_String,length)==AIO_DF_TimeOut) goto AIO_HANDSHAKING_Re_filter;      
   if(Protocol_Filter(AIO_SERVICE_TIME_10ms,Stop_Code,Size_Stop_Code)==AIO_DF_TimeOut)
   {
      AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Error_Abort,Stop_Code);
      goto AIO_HANDSHAKING_Re_filter;
   } 
}

/***************************************************************************************************/ 
/*-------------------------------------------------------------------------------------------------*/    
void UI_AllInOneTest::TestItem_Stylization(U8 Pump)
{
   U8 i,j,offset;
   U8 TI_temp[AIO_Size_TI_Number];

   ClearTestItem(); 
        
   for(i=0; i<=Pump; i++)  
   {
      (i==0)? offset=AIO_Size_TI_Code : offset +=(AIO_Size_TI_Number + AIO_Size_S_Code);      
      for(j=0; j<AIO_Size_TI_Number; j++) TI_temp[j]=AIO_Buffer[offset+j]-'0';         
      TestItem_Queue[i]= TI_temp[0]*100 + TI_temp[1]*10 + TI_temp[2];  
   }
}

/***************************************************************************************************/ 
/*-------------------------------------------------------------------------------------------------*/
U8 UI_AllInOneTest::Get_TestItem(void)
{
   U8  i,offset,Tachometer,Rangefinder,Ticker;
   U8  siftings,AIO_data=0,TimeOutFlag;
   S8  CRC_temp[AIO_Size_CS_CRC16];   
   U16 Runtime_CRC,Protocol_CRC;     
       
       
   AIO_TestItem_Re_Get:
   /******************/     
   ClearBuffer();                         
   Protocol_Filter(0,Leader_Code,Size_Leader_Code);    
   
   siftings=Protocol_Dual_Filter(AIO_SERVICE_TIME_125ms,TestItem_Code,AIO_Resend_Code,2);   
   switch(siftings)
   {
      case AIO_DF_TimeOut:
           goto AIO_TestItem_Re_Get;           
           
      case AIO_DF1_SUCCESS:
           AIO_Buffer[0]='T';
           AIO_Buffer[1]='I';   
           break;
           
      case AIO_DF2_SUCCESS:       
           if(Protocol_Filter(AIO_SERVICE_TIME_10ms,AIO_Result_Code,AIO_Size_Result_Code)==AIO_DF_TimeOut)
           {
              AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Error_Abort,Stop_Code);      
              goto AIO_TestItem_Re_Get;
           }
           
           if(Protocol_Filter(AIO_SERVICE_TIME_10ms,Stop_Code,Size_Stop_Code)==AIO_DF_TimeOut)
           {
              AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Error_Abort,Stop_Code);      
              goto AIO_TestItem_Re_Get;
           }          
           
           if(LastStatus.TriggerFlag==1)
           {
              if(LastStatus.Last_Checksum!=0)             
                 AIO_TX("%s%s%.4X%s\r\n\r",Leader_Code,LastStatus.Last_ErrorCode,LastStatus.Last_Checksum,Stop_Code); 
              else                 
                 AIO_TX("%s%s%s\r\n\r",Leader_Code,LastStatus.Last_ErrorCode,Stop_Code); 
           }
           
           goto AIO_TestItem_Re_Get;
   }
   
   Ticker=0;
   Rangefinder=AIO_Size_Single_TI;
   Tachometer=AIO_Size_TI_Code + AIO_Size_TI_Number;             
    
   for(i=AIO_Size_TI_Code; i<Rangefinder; i++)
   {                    
      while(AIO_RX(&AIO_data)==FAILURE)    
      {
         if(TimeOutFlag=Check_timer(&AIO_Timer)) goto AIO_TestItem_Re_Get;           
      }                
             
      if((i==Tachometer) && (AIO_data==Succession_Code))       
      {
         Tachometer   = i + AIO_Size_TI_Number + AIO_Size_S_Code;
         Rangefinder += (AIO_Size_S_Code + AIO_Size_TI_Number);
         Ticker++;      
         if(Ticker > AIO_Size_Maximum_S)
         {
            AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Error_Overload,Stop_Code);
            goto AIO_TestItem_Re_Get;          
         } 
      }      
      AIO_Buffer[i]=AIO_data;                   
   }         
      
   if(Protocol_Filter(AIO_SERVICE_TIME_10ms,Stop_Code,Size_Stop_Code)==AIO_DF_TimeOut)
   {
      AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Error_Abort,Stop_Code);      
      goto AIO_TestItem_Re_Get;
   }      
    
   /***********************************************************************/         
   offset= AIO_Size_TI_Code + AIO_Size_TI_Number + Ticker * (AIO_Size_S_Code + AIO_Size_TI_Number);      
   for(i=0; i<AIO_Size_CS_CRC16; i++) CRC_temp[i]=AIO_Buffer[offset+i];
      
   sscanf(CRC_temp,"%x",&Protocol_CRC);  
   //AIO_TX("\n\rProtocol_CRC=%.4X",Protocol_CRC);    
   
   /***********************************************************************/        
   Runtime_CRC=CalcCRC16((U8*)AIO_Buffer,offset,0,0,0,0);       
   //AIO_TX("\n\rRuntime_CRC=%.4X\n\r",Runtime_CRC);
           
   if(Protocol_CRC != Runtime_CRC) 
   {
      AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Error_CRC,Stop_Code);            
      goto AIO_TestItem_Re_Get;       
   }   
   
   TestItem_Stylization(Ticker);   
   AIO_TX("%s%s",Leader_Code,TestItem_Code);        
   for(i=0; i<=Ticker ; i++)  
   {      
      AIO_TX("%.3d",TestItem_Queue[i]);              
      if((Ticker>0) && (i!=Ticker)) AIO_TX("%c",Succession_Code);        
   }   
   AIO_TX("%s\r\n\r",Stop_Code);  
                 
   return Ticker;
}

/***************************************************************************************************/ 
/*-------------------------------------------------------------------------------------------------*/
void UI_AllInOneTest::INIT(void)
{     
   U32 ulError;
   
   lib.rs232.SetUartMode(BLT_SIE);
   lib.rs232.DisableOutput(); 
   ulError=ClearRS232Buffer(); 
   if(ulError==FAILURE)
   {
      AIO_TX("< Error > AIO initiation failed.");
      while(1){}
   }
   
   ClearTestItem();
   ClearBuffer();    
   
   LastStatus.TriggerFlag=0;
   LastStatus.Last_ErrorCode=NULL;
   LastStatus.Last_Checksum=0;       
}

/***************************************************************************************************/ 
/*-------------------------------------------------------------------------------------------------*/
void UI_AllInOneTest::Handler(CHAR *ProductName,U32 Max_item)
{
   U8  i,j,k,TI_Quantity,TestItem;
   S8  Return_String[20],Error_Code_String[4];
   U16 Checksum,Error_Code;
   U32 Test_Result;            
   
   //modify Alan lib.led.Display("Hunt");
   HANDSHAKING(ProductName);     
   //modify Alan lib.led.Display("Link");
      
   while(1)
   {        
      AIO_Test_Item_Skip:
      /*****************/            
      Error_Code=0;         
      TI_Quantity=Get_TestItem();     
      for(i=0; i<=TI_Quantity; i++) 
      {      
         TestItem=TestItem_Queue[i];    
         if(TestItem > Max_item )
         {
            AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Error_Null,Stop_Code);      
            goto AIO_Test_Item_Skip;
         }    
      
/*************************************************************************/      
#if 0         
         if(Get_TI_License(TestItem)==FAIL) 
         {
            AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Error_Lock,Stop_Code); 
            goto AIO_Test_Item_Skip;
         } 
#endif         
/*************************************************************************/         
         OutMsg()->RunningItem(TestItem);        
         
         Test_Result=RunItem(TestItem, 0);
         
         LastStatus.TriggerFlag=1;      
         switch(Test_Result)
         {
            case AIO_Test_SUCCESSFUL:
                 if(TI_Quantity==0) 
                 {
                    LastStatus.Last_ErrorCode=AIO_Test_Pass;
                    LastStatus.Last_Checksum=0; 
                    OutMsg()->TestResult(0); 
                    AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Test_Pass,Stop_Code);
                    AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Test_Pass,Stop_Code);
                    AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Test_Pass,Stop_Code);
                 }   
                 break;
                  
            case AIO_Test_FAILED:
                 if(TI_Quantity==0) 
                 {
                    LastStatus.Last_ErrorCode=AIO_Test_Fail;
                    LastStatus.Last_Checksum=0; 
                    OutMsg()->TestResult(1);
                    AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Test_Fail,Stop_Code); 
                    AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Test_Fail,Stop_Code); 
                    AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Test_Fail,Stop_Code); 
                 }   
                 else Error_Code |=(1<<i);
                 break;
             
            case AIO_Test_SKIPPABLE:
                 if(TI_Quantity==0) 
                 {
                    LastStatus.Last_ErrorCode=AIO_Error_Null;
                    LastStatus.Last_Checksum=0; 
                    OutMsg()->TestResult(2);
                    AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Error_Null,Stop_Code);
                 }
                 break;
                  
             default:     
                 AIO_TX("< Error > Unidentified test item return values!\r\n\r");
                 while(1){}
         }                                                          
      }//End for-loop()
      
      if(TI_Quantity>0)
      {    
         if(Error_Code != SUCCESS)
         {       
            j=0;             
            for(k=0; AIO_Test_Fail[k]!='\0'; j++,k++) Return_String[j]= AIO_Test_Fail[k];                          
            Return_String[j++]=Succession_Code;            
            sprintf(Error_Code_String,"%.4X",Error_Code);
            for(k=0; Error_Code_String[k]!='\0'; j++,k++) Return_String[j]= Error_Code_String[k];            
            Return_String[j]='\0'; // Return_String="T-FAIL+????\0"            
            Checksum=CalcCRC16((U8*)Return_String,j,0,0,0,0); /* To get CRC16 value */                         
            LastStatus.Last_ErrorCode=Return_String;
            LastStatus.Last_Checksum=Checksum;            
            AIO_TX("%s%s%.4X%s\r\n\r",Leader_Code,Return_String,Checksum,Stop_Code);   
         }   
         else
         {
            LastStatus.Last_ErrorCode=AIO_Test_Pass;
            LastStatus.Last_Checksum=0; 
            AIO_TX("%s%s%s\r\n\r",Leader_Code,AIO_Test_Pass,Stop_Code);
         }  
		 lib.peripheral.WatchDog_Serve();         
      }     
   }//End while-loop()   
}//End AIO_Station_Test()


/***************************************************************************************************/ 
/*-------------------------------------------------------------------------------------------------*/
void UI_AllInOneTest::Run()
{
	bool rs232enabled = lib.rs232.DisableOutput();
	INIT();
	Handler(CONFIG_STB_MODEL_SIE,256);
	if (rs232enabled) {	// restore RS232 status
		lib.rs232.EnableOutput();
	}
}

