#include "TiApi.h"
#include "mainmenumgr.h"
#include "TiLib.h"
#include "addrutil.h"
#include "bvsb.h"
#include "Mot7400_TunerController.h"

#define BCHP_SUN_TOP_CTRL_PROD_REVISION          0x00404000 /* Product Revision ID */
#define BCM_DEMOD_TYPE					0x3517
#define BCM_DEMOD_REV_B0				0x02

extern "C" BVSB_Handle                hVSB[MAX_NXP_TUNERS];

//##ModelId=46EB469302C9
U32 TiApi::ChipID_Get(PeripheralChip chip, U32* id)
{
	U32 lError = 0;
	
	switch (chip)
	{	
	 case CT_MAIN_CHIP:
	     *id=lib.chipID.GetMainChip( GetBcmProcessorBase() | BCHP_SUN_TOP_CTRL_PROD_REVISION);
	     break;
	 
	 case CT_DEMOD:
//    	 *id=lib.chipID.GetPCIDev(CT_DEMOD);
    	 break;
    	 
     case CT_1394CHIP:
         *id=lib.chipID.GetPCIDev(CT_1394CHIP);
       	 break;
         
       	 //lError=
     //case CT_MPEG2_ENCODER:
     //case CT_ENTROPIC:
     case CT_BOARD_ID:
       	 *id=lib.chipID.GetBoardID();		
     	 break;
     case CT_PSOC_FAN://Check PSOC FW version
       	 *id=lib.chipID.GetI2CDev(CT_PSOC_FAN);
       	 break;
     //case CT_PSOC_FP: //for DCX3425 only
	}
	
	return lError;
}

U32 TiApi::ChipID_Demod_Check()
{
	U32 bcmDemodType = 0;
	BERR_Code eErr1, eErr2;
	//S8 ucVer1[6];
	U8 ucVer1, ucVer2;
	U32 lError = 1;
	
	bcmDemodType = GetBcmDemodType();
	if (bcmDemodType == BCM_DEMOD_TYPE) {
		if (lib.IsProductModel("QIP72XXP2")) {
			eErr1 = BVSB_GetChipRevision(hVSB[0], &ucVer1);
			eErr2 = BVSB_GetChipRevision(hVSB[1], &ucVer2);
			if (eErr1 == BERR_SUCCESS && eErr2 == BERR_SUCCESS && ucVer1 == BCM_DEMOD_REV_B0 && ucVer2 == BCM_DEMOD_REV_B0) {
				lError = 0;
			}
		}
		else {
			eErr1 = BVSB_GetChipRevision(hVSB[0], &ucVer1);
			if (eErr1 == BERR_SUCCESS && ucVer1 == BCM_DEMOD_REV_B0) {
				lError = 0;
			}
		}
		
/*		GetBcmDemodRevision(ucVer);
		if (strcmp(ucVer, BCM3517_REV) == 0) {
			lError = 0;
		}*/
	}
	PDEBUG( "Demod Type                     %s%x\r\n","Bcm",GetBcmDemodType() );
//	PDEBUG( "Demod Version                  %s\r\n",ucVer );
	PDEBUG( "Demod1 Version                 0X%.2X\r\n",ucVer1 );
	if (lib.IsProductModel("QIP72XXP2")) {
		PDEBUG( "Demod2 Version                 0X%.2X\r\n",ucVer2 );
	}
	
	return lError;
}

extern "C" {
	unsigned int Bcm740X_Get_DeviceId(unsigned char *pucDeviceId, unsigned int uiBufLen);
	unsigned char Bcm740X_Get_JtagLockStatus();
	unsigned char Bcm740X_Get_BackgroundCheckStatus();
	unsigned char Bcm740X_Get_SecureBootStatus();
	unsigned char Bcm740X_Get_SecureBootSize();
	unsigned char Bcm740X_Get_BoundaryScanStatus();
	void Bcm740X_ProgramOtpBits_JtagAuthLock();
	void Bcm740X_ProgramOtpBits_BackgroundCheck(unsigned char ucSetEnabled);
	void Bcm740X_ProgramOtpBits_BoundaryScanLock();
}

DeviceSecurityStatus_t TiApi::CPU_OTP_GetSecurityStatus()
{
	DeviceSecurityStatus_t DevSecStat;

	DevSecStat.JtagStatus = (SS_JtagStatus_e) Bcm740X_Get_JtagLockStatus();
	DevSecStat.SecureBootStatus = (SS_SecureBootStatus_e) Bcm740X_Get_SecureBootStatus();
	DevSecStat.SecureBootSize = (SS_SecureBootSize_e) Bcm740X_Get_SecureBootSize();
	DevSecStat.BkGndChkStatus = (SS_eBkGndChkStatus_e) Bcm740X_Get_BackgroundCheckStatus();
	DevSecStat.BndryScanStatus = (SS_eBndryScanStatus_e) Bcm740X_Get_BoundaryScanStatus();
	
	return DevSecStat;
}

U32 TiApi::CPU_OTP_LockVerify()
{
	U32 lError = 0;
	
	DeviceSecurityStatus_t DevSecStat;

	DevSecStat.JtagStatus = (SS_JtagStatus_e) Bcm740X_Get_JtagLockStatus();
	DevSecStat.BkGndChkStatus = (SS_eBkGndChkStatus_e) Bcm740X_Get_BackgroundCheckStatus();
	DevSecStat.BndryScanStatus = (SS_eBndryScanStatus_e) Bcm740X_Get_BoundaryScanStatus();

	if (DevSecStat.JtagStatus != JTAG_STATUS_REQUIRES_AUTH) {
		PDEBUG(("Permanently locking out JTAG pins until authentication.\r\n"));
	}
	if (DevSecStat.BkGndChkStatus != BKGND_CHECK_ENABLED) {
		PDEBUG(("Permanently enabling background checker\r\n"));
	}
	if (DevSecStat.BndryScanStatus != BNDRY_SCAN_DISABLED) {
		PDEBUG(("Permanently locking out Boundary Scan pins until authentication.\r\n"));
	}
	
	if ( (DevSecStat.JtagStatus == JTAG_STATUS_REQUIRES_AUTH) && (DevSecStat.BkGndChkStatus == BKGND_CHECK_ENABLED) && (DevSecStat.BndryScanStatus == BNDRY_SCAN_DISABLED) ) {
		lError = 0;
	}
	else {
		lError = 1;
	}
	
	return lError;
}

//##ModelId=482BA3DA032C
U32 TiApi::CPU_OTP_Lock()
{
	DeviceSecurityStatus_t DevSecStat;

	DevSecStat.JtagStatus = (SS_JtagStatus_e) Bcm740X_Get_JtagLockStatus();
	DevSecStat.BkGndChkStatus = (SS_eBkGndChkStatus_e) Bcm740X_Get_BackgroundCheckStatus();
	DevSecStat.BndryScanStatus = (SS_eBndryScanStatus_e) Bcm740X_Get_BoundaryScanStatus();

	if (DevSecStat.JtagStatus != JTAG_STATUS_REQUIRES_AUTH) {
		PDEBUG(("Permanently locking out JTAG pins until authentication.\r\n"));
		Bcm740X_ProgramOtpBits_JtagAuthLock();
	}
	if (DevSecStat.BkGndChkStatus != BKGND_CHECK_ENABLED) {
		PDEBUG(("Permanently enabling background checker\r\n"));
		Bcm740X_ProgramOtpBits_BackgroundCheck(1); //Pass a 1 to program the backgnd check as enabled.
	}
	if (DevSecStat.BndryScanStatus != BNDRY_SCAN_DISABLED) {
		PDEBUG(("Permanently locking out Boundary Scan pins until authentication.\r\n"));
		Bcm740X_ProgramOtpBits_BoundaryScanLock();
	}
	
	return 0;
}

extern void GetDeviceSecurityStatus();

//##ModelId=482BA3F2034B
void  TiApi::CPU_OTP_DisplayProcessorSecurityStatus()
{
	GetDeviceSecurityStatus();
}

//##ModelId=472C54F202CF
U32 TiApi::IEEE1394OHCI_Test()
{
	U32 lError = 0;
	
	lError = lib.chipID.OHCI_Test();
	
	return lError;
}


//##ModelId=471700A103A7
void TiApi::EnterToTvMonMenu()
{
	bool rs232enabled = lib.rs232.EnableOutput();
	
	tvMonIO_toRS232();
	CommandLineProcess();
	tvMonIO_toSDRAM();
	
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
}

