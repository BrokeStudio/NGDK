#include "NGDK.h"
#include "asm.h"

#include "NG_arge.h"


void NG_FixPal(u16 pal)
{
	NG_PAL_ID = pal;
}

void __attribute__((noinline)) NG_PrintFix(u16 val,u16 x,u16 y)
{
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;
	NG_arg3_u16 = val;

	asm (
		"movem.l %d0-%d1,-(%a7)\n	"

		"move.w	NG_arg1_u16,%d0\n	"
		"asl.w	#5,%d0\n	"
		"add.w  #FIXMAP+0x22,%d0\n	"
		"add.w	NG_arg2_u16,%d0\n	"
		"move.w %d0,VRAM_ADDR\n	"

		"move.w	NG_PAL_ID,%d0\n	"
		"add.w	NG_arg3_u16,%d0\n	"
		"move.w %d0,VRAM_RW\n	"

		"movem.l (%a7)+,%d0-%d1\n	"
		);
}

void __attribute__((noinline)) NG_Print(const void *str,u16 x,u16 y)
{
	NG_arg1_u32 = str;
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;

	asm (
		"movem.l %d0-%d2/%a0-%a1,-(%a7)\n	"
		"move.w  #0x20,VRAM_MOD\n	"

		"move.w	NG_arg1_u16,%d1\n	"
		"asl.w	#5,%d1\n	"
		"add.w  #FIXMAP+0x22,%d1\n	"
		"add.w	NG_arg2_u16,%d1\n	"

		"move.w	NG_PAL_ID,%d2\n	"
		"add.w	#FIX_ASCII,%d2\n	"
		"move.w  %d1,VRAM_ADDR\n	"

		"move.l NG_arg1_u32,%a0\n	"
		"lea VRAM_RW,%a1\n	"

		"clr.w  %d1\n	"
		"NG_Print_loop:\n	"
			"clr.w  %d0\n	"
			"move.b (%a0)+,%d0\n	"

			//"cmp.w  %d1,%d0\n	"
			"beq.w NG_Print_end\n	"

			"add.w	%d2,%d0\n	"
			"move.w %d0,(%a1)\n	"
		"bne.w NG_Print_loop\n	"
		"NG_Print_end:\n	"

		"movem.l (%a7)+,%d0-%d2/%a0-%a1\n	"
		);
}

void __attribute__((noinline)) NG_PrintCPU(u16 x,u16 y)
{
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;

	asm (
		"movem.l %d0-%d4/%a0,-(%a7)\n	"
		"move.w  #0x20,VRAM_MOD\n	"

		"move.w	NG_arg1_u16,%d0\n	"
		"asl.w	#5,%d0\n	"
		"add.w  #FIXMAP+0x22,%d0\n	"
		"add.w	NG_arg2_u16,%d0\n	"
		"move.w  %d0,VRAM_ADDR\n	"

		"move.w NG_RAM+2,%d2\n	"
		"move.w %d2,%d1\n	"
		"move.w #100,%d0\n	"
		"move.w %d0,%d4\n	"

		"asr.w #8,%d2\n	"
		"asr.w #5,%d1\n	"
		"add.w %d1,%d2\n	"
		"sub.w %d2,%d0\n	"
		"move.w %d0,%d2\n	"

		"move.w	NG_PAL_ID,%d3\n	"
		"add.w	#FIX_ASCII+'0',%d3\n	"

		"lea VRAM_RW,%a0\n	"


		"move.w %d3,%d0\n	"
		"digit_convert 101\n	"
		"move.w  %d0,(%a0)\n	"

		"digit_convert10 100\n	"

		"move.w %d3,%d0\n	"
		"add.w	%d2,%d0\n	"
		"move.w %d0,(%a0)\n	"

		"move.w %d3,%d0\n	"
		"sub.w	#11,%d0\n	"
		"move.w %d0,(%a0)\n	"

		"movem.l (%a7)+,%d0-%d4/%a0\n	"
		);
}