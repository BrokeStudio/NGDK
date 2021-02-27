#include "NGDK.h"
#include "asm.h"

#include "NG_arge.h"

void NG_Fix_Print_h32(u32 val, u16 x, u16 y)
{
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;
	NG_arg3_u32 = val;

	asm (
		"movem.l %d2-%d4,-(%a7)\n	"

		"move.w  #0x20,VRAM_MOD\n	"

		"move.w	NG_arg1_u16,%d1\n	"
		"asl.w	#5,%d1\n	"
		"add.w  #FIXMAP+0x22,%d1\n	"
		"add.w	NG_arg2_u16,%d1\n	"
		"move.w  %d1,VRAM_ADDR\n	"


		"move.w	NG_PAL_ID,%d3\n	"
		"add.w	#FIX_ASCII+'0',%d3\n	"
		"move.w	%d3,%d4\n	"
		"add.w	#'A'-'0'-10,%d4\n	"

		"lea VRAM_RW,%a0\n	"

		"move.w NG_arg3_u32,%d2\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#8,%d1\n	"
		"asr.w	#4,%d1\n	"
		"hexa_convert 40\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#8,%d1\n	"
		"hexa_convert 41\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#4,%d1\n	"
		"hexa_convert 42\n	"

		"mov.w	%d2,%d1\n	"
		"hexa_convert 43\n	"

		"move.w NG_arg3_u32+2,%d2\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#8,%d1\n	"
		"asr.w	#4,%d1\n	"
		"hexa_convert 44\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#8,%d1\n	"
		"hexa_convert 45\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#4,%d1\n	"
		"hexa_convert 46\n	"

		"mov.w	%d2,%d1\n	"
		"hexa_convert 47\n	"


		"movem.l (%a7)+,%d2-%d4\n	"
		);
}
