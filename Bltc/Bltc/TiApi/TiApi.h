#ifndef TIAPI_H_HEADER_INCLUDED_B9049B16
#define TIAPI_H_HADER_INCLUDED_B9049B16
#include "TiApi_OurMenu.h"
#include "TiApi_DigAV.h"
#include "TiApi_FrontEnd.h"
#include "TiApi_Peripheral.h"
#include "TiApi_MCard.h"
#include "TiApi_Memory.h"
#include "TiApi_TunerTest.h"
#include "ModelConfig.h"
#include "TiLib_Chip.h"

// Follows copy from OtpMenu.c
typedef enum eJtagStatus
{
	JTAG_STATUS_UNKNOWN,
	JTAG_STATUS_UNLOCKED,
	JTAG_STATUS_REQUIRES_AUTH,
	JTAG_STATUS_DISABLED
} SS_JtagStatus_e;

typedef enum eBackgroundCheckStatus
{
	BKGND_CHECK_UNKNOWN,
	BKGND_CHECK_DISABLED,
	BKGND_CHECK_ENABLED,
} SS_eBkGndChkStatus_e;

typedef enum eSecureBootStatus
{
	SECURE_BOOT_UNKNOWN,
	SECURE_BOOT_DISABLED,
	SECURE_BOOT_ENABLED,

} SS_SecureBootStatus_e;

typedef enum eSecureBootSize
{
	SECURE_BOOT_SIZE_8KBYTES 	= 0,
	SECURE_BOOT_SIZE_16KBYTES	= 1,
	SECURE_BOOT_SIZE_32KBYTES	= 2,
	SECURE_BOOT_SIZE_64KBYTES	= 3,
	SECURE_BOOT_SIZE_128KBYTES	= 4,
	SECURE_BOOT_SIZE_256KBYTES	= 5,
	SECURE_BOOT_SIZE_512KBYTES	= 6,
	SECURE_BOOT_SIZE_1024KBYTES = 7,
	SECURE_BOOT_SIZE_INVALID			
} SS_SecureBootSize_e;


typedef enum SS_eBndryScanStatus
{
	BNDRY_SCAN_UNKNOWN,
	BNDRY_SCAN_DISABLED,
	BNDRY_SCAN_ENABLED,
} SS_eBndryScanStatus_e;

typedef struct DeviceSecurityStatus
{
	SS_JtagStatus_e			JtagStatus;
	SS_SecureBootStatus_e	SecureBootStatus;	
	SS_SecureBootSize_e		SecureBootSize;
	SS_eBkGndChkStatus_e	BkGndChkStatus;
	SS_eBndryScanStatus_e	BndryScanStatus;
} DeviceSecurityStatus_t;

//##ModelId=46EB464C034A
class TiApi
{
  public:
    //##ModelId=46EB469302C9
    U32 ChipID_Get(PeripheralChip chip, U32* id);
    U32 ChipID_Demod_Check();
    DeviceSecurityStatus_t CPU_OTP_GetSecurityStatus();
    U32 CPU_OTP_LockVerify();
    //##ModelId=482BA3DA032C
    U32 CPU_OTP_Lock();
    //##ModelId=482BA3F2034B
    void CPU_OTP_DisplayProcessorSecurityStatus();
    //##ModelId=472C54F202CF
	U32 IEEE1394OHCI_Test();
    //##ModelId=471700A103A7
    void EnterToTvMonMenu();

    //##ModelId=48167D9801E4
	TiApi_OurMenu menu;
    //##ModelId=46F354A400F0
    TiApi_DigAV digAV;
    //##ModelId=46F36763025D
    TiApi_FrontEnd frontEnd;
    //##ModelId=46F3677300C3
    TiApi_Peripheral peripheral;
    //##ModelId=46F3677D0053
    TiApi_MCard mCard;
    //##ModelId=46F36788015B
    TiApi_Memory memory;
#ifdef CONFIG_WITH_TUNER_TEST
    TiApi_TunerTest tunerTest;
#endif // #ifdef CONFIG_WITH_TUNER_TEST
};

extern TiApi api;

#endif /* TIAPI_H_HEADER_INCLUDED_B9049B16 */
