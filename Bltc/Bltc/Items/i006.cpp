// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiLib.h"
#include "MipsCpu.h"

//#include "vr5432.h"

#if 0
asm void __DumpRegisters(void)
{

	// start register dump
    li      t0, UNCACHED(0x1F007000)  	// load address to t0

    // save general purpose registers to end of NVM (QUOD SDRAM)
    sw      zero,0x00(t0)
    sw      $at, 0x04(t0)
    sw      v0,  0x08(t0)
    sw      v1,  0x0C(t0)
    sw      a0,  0x10(t0)
    sw      a1,  0x14(t0)
    sw      a2,  0x18(t0)
    sw      a3,  0x1C(t0)

    // t0 contains the dump address of this function so it is not very 
    // useful but we dump it anyhow.
    sw      t0,  0x20(t0)
    
    // t1 was used by the TimerDelay function so it is not very 
    // useful but we dump it anyhow.
    sw      t1,  0x24(t0)

    // continue saving the GPRs 
    sw      t2,  0x28(t0) 
    sw      t3,  0x2C(t0)
    sw      t4,  0x30(t0)
    sw      t5,  0x34(t0)
    sw      t6,  0x38(t0)
    sw      t7,  0x3C(t0)
    sw      s0,  0x40(t0)
    sw      s1,  0x44(t0)
    sw      s2,  0x48(t0)
    sw      s3,  0x4C(t0)
    sw      s4,  0x50(t0)
    sw      s5,  0x54(t0)
    sw      s6,  0x58(t0)
    sw      s7,  0x5C(t0)
    sw      t8,  0x60(t0)
    sw      t9,  0x64(t0)
    sw      k0,  0x68(t0) 
    sw      k1,  0x6C(t0)
    sw      gp,  0x70(t0)
    sw      sp,  0x74(t0)  
    sw      fp,  0x78(t0)

    // The current ra register 
    // only holds the current function calls return address.
    sw      ra,  0x7C(t0)  
            
    // Now we dump the CP0 registers into NVM.
    // Note: RFU (Reserved for future use) register 
    //   are not stored by this code and are zeroed out.
    //
    mfc0    k1,  C0_INX
    sw      k1,  0x80(t0)
    mfc0    k1,  C0_RAND
    sw      k1,  0x84(t0)
    mfc0    k1,  C0_TLBLO0
    sw      k1,  0x88(t0)
    mfc0    k1,  C0_TLBLO1
    sw      k1,  0x8C(t0)
    mfc0    k1,  C0_CTEXT
    sw      k1,  0x90(t0)
    mfc0    k1,  C0_PGMASK
    sw      k1,  0x94(t0)
    mfc0    k1,  C0_WIRED
    sw      k1,  0x98(t0)

    //  Skip CP0 reg 7 RFU
    sw      zero,0x9C(t0)
    
    mfc0    k1,  C0_BADVADDR 
    sw      k1,  0xA0(t0)
    mfc0    k1,  C0_COUNT
    sw      k1,  0xA4(t0)
    mfc0    k1,  C0_TLBHI
    sw      k1,  0xA8(t0)
    mfc0    k1,  C0_COMPARE
    sw      k1,  0xAC(t0)
    mfc0    k1,  C0_STATUS
    sw      k1,  0xB0(t0)
    mfc0    k1,  C0_CAUSE
    sw      k1,  0xB4(t0)
    mfc0    k1,  C0_EPC
    sw      k1,  0xB8(t0)
    mfc0    k1,  C0_PRID
    sw      k1,  0xBC(t0)
    mfc0    k1,  C0_CONFIG
    sw      k1,  0xC0(t0)
    
    //	CP0 reg 17 RFU
    sw      zero,  0xC4(t0)
    
    mfc0    k1,  C0_WATCHLO
    sw      k1,  0xC8(t0)
    mfc0    k1,  C0_WATCHHI
    sw      k1,  0xCC(t0)
    mfc0    k1,  C0_XCTEXT
    sw      k1,  0xD0(t0)
    
    // Skip CP0 reg 21-24 RFU
    sw      zero,0xD4(t0)
    sw      zero,0xD8(t0)
    sw      zero,0xDC(t0)
    sw      zero,0xE0(t0)
    
    mfc0    k1,  C0_PERFCNT
    sw      k1,  0xE4(t0)
    mfc0    k1,  C0_ECC
    sw      k1,  0xE8(t0)
    mfc0    k1,  C0_CACHEERR
    sw      k1,  0xEC(t0)
    mfc0    k1,  C0_TAGLO
    sw      k1,  0xF0(t0)
    mfc0    k1,  C0_TAGHI
    sw      k1,  0xF4(t0)
    mfc0    k1,  C0_ERREPC
    sw      k1,  0xF8(t0)
    
    // Skip C0 reg 31 RFU
    sw      zero,0xFC(t0)
    
    // Skip Log Offsets 0x100 - 0x10c for Legacy BCM7035 regs.
    sw		zero,0x100(t0)
    sw		zero,0x104(t0)
    sw		zero,0x108(t0)
    sw		zero,0x10C(t0)    
	
	//////////////////////////////////////////
	// 		BCM7038 Specific Regs			//
	//////////////////////////////////////////
    li		v0,	UNCACHED(BCHP_PHYSICAL_OFFSET)
	
	// BCHP_HIF_CPU_INTR1_INTR_W0_STATUS
    lw		v1,	0x00(v0)
    sw		v1,	0x110(t0)
    
	// BCHP_HIF_CPU_INTR1_INTR_W1_STATUS
    lw		v1,	0x04(v0)
    sw		v1,	0x114(t0)
    
	// BCHP_HIF_CPU_INTR1_INTR_W0_MASK_STATUS
    lw		v1,	0x08(v0)
    sw		v1,	0x118(t0)
    
	// BCHP_HIF_CPU_INTR1_INTR_W1_MASK_STATUS
    lw		v1,	0x0C(v0)
    sw		v1,	0x11C(t0)
    
 EXIT:
    jr      ra
    nop

} /* endof __DumpRegisters */
#endif

U32 tmp0;

U32 Items::TI006()
{
#if 1
	bool rs232enabled = lib.rs232.EnableOutput();
	U32* p = (U32*)0xB0400B00;
	
	asm {
		mfc0	t0, Cause
		sw t0, tmp0
		nop
		nop
		nop
	}
	lib.rs232.Print("Cause = %.8X\r\n", tmp0);
	asm {
		mfc0	t0, SR
		sw t0, tmp0
		nop
		nop
		nop
	}
	lib.rs232.Print("SR = %.8X\r\n", tmp0);
	//__DumpRegisters();
	lib.rs232.Print("0xB0400B00 = %.8X\r\n", *p);
	p++;
	lib.rs232.Print("0xB0400B04 = %.8X\r\n", *p);
	p++;
	lib.rs232.Print("0xB0400B08 = %.8X\r\n", *p);
	p++;
	lib.rs232.Print("0xB0400B0C = %.8X\r\n", *p);
	p++;
	lib.rs232.Print("0xB0400B10 = %.8X\r\n", *p);
	p++;
	lib.rs232.Print("0xB0400B14 = %.8X\r\n", *p);
	p++;
	lib.rs232.Print("0xB0400B18 = %.8X\r\n", *p);
	p++;
	lib.rs232.Print("0xB0400B1C = %.8X\r\n", *p);
	p++;

	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
	
	lib.DelayMS(2000);
#endif
	
	return TI_SUCCESS;
}
