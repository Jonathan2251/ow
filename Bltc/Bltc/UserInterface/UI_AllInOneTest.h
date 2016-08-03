#include "UserInterface.h"
#include "TiLib.h"


#define Leader_Code       "@{"
#define Size_Leader_Code  2

#define Stop_Code         "}@"
#define Size_Stop_Code    2


#define TestItem_Code     "TI"
#define Succession_Code   '+'
enum AIO_Test_Item_Protocol_Length
{
  AIO_Size_TI_Code    = 2,  //(Byte) , "TI"  , "TI" is the abbreviation of "Test Item".  
  AIO_Size_TI_Number  = 3,  //(Byte) , "XXX" , the "XXX" mean for item number 000 ~ 999.  
  AIO_Size_Maximum_TI = 16,  
  AIO_Size_S_Code     = 1,  //(Byte) , "+"   , Succession Code    
  AIO_Size_Maximum_S  = 15,  
  AIO_Size_CS_CRC16   = 4, 
  AIO_Size_Single_TI  = AIO_Size_TI_Code + AIO_Size_TI_Number + AIO_Size_CS_CRC16,  
  AIO_Size_Multiple_TI= AIO_Size_TI_Code + AIO_Size_TI_Number * AIO_Size_Maximum_TI + AIO_Size_S_Code * AIO_Size_Maximum_S + AIO_Size_CS_CRC16  
};


#define AIO_Resend_Code       "R-"
#define AIO_Result_Code       "RESULT"
#define AIO_Size_Resend_Code  2  
#define AIO_Size_Result_Code  6


typedef enum _AIO_Test_Result_Type
{
   AIO_Test_SUCCESSFUL=0,
   AIO_Test_FAILED,
   AIO_Test_SKIPPABLE
}  AIO_Test_Result;



#define AIO_Test_Ready        "T-READY"
#define AIO_Test_Pass         "T-PASS"
#define AIO_Test_Fail         "T-FAIL"
#define AIO_Error_CRC         "E-CRC"
#define AIO_Error_Abort       "E-ABORT"
#define AIO_Error_Null        "E-NULL" 
#define AIO_Error_Lock        "E-LOCK" 
#define AIO_Error_Overload    "E-OVERLOAD"

#define AIO_SERVICE_TIME_HOLD            0*ADJUST_CPU_CLOCK_RATE
#define AIO_SERVICE_TIME_10ms        10000*ADJUST_CPU_CLOCK_RATE  
#define AIO_SERVICE_TIME_125ms      125000*ADJUST_CPU_CLOCK_RATE
#define AIO_SERVICE_TIME_1s        1000000*ADJUST_CPU_CLOCK_RATE 


typedef enum _AIO_Dual_Filter_Error_Code
{
   AIO_DF_TimeOut=0,
   AIO_DF0_SUCCESS,
   AIO_DF1_SUCCESS,
   AIO_DF2_SUCCESS   
}  AIO_DF_ErrCode;

typedef struct _AIO_Last_ErrorCode
{
   U8   TriggerFlag;
   S8  *Last_ErrorCode;
   U16  Last_Checksum;  
}  AIO_LAST_EC;


class UI_AllInOneTest : public UserInterface
{
  public:
  	UI_AllInOneTest(UI_OutputMsg *outMsg);
    void Run();
    
  private:  
    S8  AIO_Buffer[AIO_Size_Multiple_TI];
    U8  TestItem_Queue[AIO_Size_Maximum_TI];
    AIO_LAST_EC LastStatus;

    U8 Get_TestItem(void);
  
    AIO_DF_ErrCode Protocol_Filter(U32 TimeOut,S8 *Protocol_Content,U8 length); 
    AIO_DF_ErrCode Protocol_Dual_Filter(U32 TimeOut,S8 *Protocol_Content_1,S8 *Protocol_Content_2,U8 length);    
    
    int AIO_TX(S8* pcFormat, ...);
    U32 AIO_RX(U8* rxByte);
    void HANDSHAKING(CHAR *ProductName);    
    void INIT(void);    
    void ClearTestItem(void);    
    void TestItem_Debug(void);    
    void ClearBuffer(void);    
    void Buffer_Debug(void);    
    void TestItem_Stylization(U8 Pump);    
    void Handler(S8 *ProductName,U32 Max_item);   
   
};

 
