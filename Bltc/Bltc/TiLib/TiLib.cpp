#include "TiLib.h"
#include "board_id.h"
#include "cputimer.h"
#include "MipsCpu.h"

//##ModelId=470200FF00ED
TiLib::TiLib() : rs232(0)
{
	tvMonIO_init();	// BLTC modify : Gamma add
}

//##ModelId=48114CDC0232
void TiLib::Init()
{
	if (lib.IsProductModel("QIP7100P2")) {
		led = new TiLib_Led_QIP7100_P2;
	}
	else if (lib.IsProductModel("QIP72XXP2")) {
		led = new TiLib_Led_QIP72XX_P2;
	}
	else {
		led = new TiLib_Led_QIP72XX_P2;
	}
	lib.led->Init();
	lib.ir.Init();
	lib.rs232.Init();
	lib.digAV.Init();
#ifdef CONFIG_WITH_OSD
	lib.osd.Init();
#endif	// CONFIG_WITH_OSD
}

//##ModelId=48C4925A007A
AsmVoid TiLib::EnableInterrupt()
{
	addiu   sp,sp,-32
	sw   t0,(0*8)(sp)
	sw   t1,(1*8)(sp)
	
	// Enable interrupt (set bit 15, clear bit 0 (IE), 
	// and set bit 0 in SR register)
	//		set t0 = 0s(31..16) | ((SR(15..0)|(0x8001))
	mfc0		t0,SR
	ori			t0,t0,0x0001
	//		set t1 = SR(31..16)|0s(15:0)
	mfc0		t1,SR
	srl			t1,t1,16
	sll			t1,t1,16
	//		set SR = t0|t2
	or			t1,t1,t0
	mtc0		t1,SR
	
	lw   t0,(0*8)(sp)
	lw   t1,(1*8)(sp)
	addiu   sp,sp,32

	jr	ra
	nop
}

//##ModelId=48C4926F004C
AsmVoid TiLib::DisableInterrupt()
{
	addiu   sp,sp,-32
	sw   t0,(0*8)(sp)
	sw   t1,(1*8)(sp)
	
	// Disable interrupt (clear bit 0 (IE), in SR register)
	//		set t0 = 0s(31..16) | ((SR(15..0)&(0xFFFE))
	mfc0		t0,SR
	andi		t0,t0,0xFFFE
	//		set t1 = SR(31..16)|0s(15:0)
	mfc0		t1,SR
	srl			t1,t1,16
	sll			t1,t1,16
	//		set SR = t0|t1
	or			t1,t1,t0
	mtc0		t1,SR
	
	lw   t0,(0*8)(sp)
	lw   t1,(1*8)(sp)
	addiu   sp,sp,32

	jr	ra
	nop
}

//##ModelId=4702E17700C9
 void TiLib::DelayMS(U32 ms)
{
	Delay_ms(ms);
//	BKNI_Delay_R(1000*ms);
}

//##ModelId=4702F2FC017C
 U32 TiLib::GetProductModel(S8* model)
{
	U32 result = FAILURE;
	U16 boardId = DetermineHardwareBoardId(FALSE);
	
	model[0] = '\0';
	switch (boardId) {
	case HWID_QIP72XX:
		strcpy(model, "QIP72XX");
		result = SUCCESS;
		break;
	case HWID_QIP7100:
		strcpy(model, "QIP7100");
		result = SUCCESS;
		break;
	case HWID_QIP7100_P2:
		strcpy(model, "QIP7100P2");
		result = SUCCESS;
		break;
	case HWID_QIP72XX_P2:
		strcpy(model, "QIP72XXP2");
		result = SUCCESS;
		break;
	}
	
	return result;
}

//##ModelId=4702F30903CE
 bool TiLib::IsProductModel(S8* model)
{
	char prodModel[100];
	bool result;
	
	if (GetProductModel(prodModel) != SUCCESS) {
		return false;
	}
	if (strcmp(model, prodModel) == 0) {
		result = true;
	}
	else {
		result = false;
	}
	return result;
}







