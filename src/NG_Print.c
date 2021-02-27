#include "NGDK.h"
#include "asm.h"

#include "NG_arge.h"


void NG_Fix_Palette(u16 pal)
{
	NG_PAL_ID = pal;
}

/**
 * Buffer format is as follow, and uses words:
 * 
 * mode, x, y, n, data, (...), [mode, x, y, n, data, (...),] 0xffff
 * 
 * mode: 0 is for horizontal update | 1 is for vertical update
 * x: X position (tile 0-39)
 * y: Y position (tile 0-27)
 * data: tile + palette data
 * 0xffff: end of buffer (anything different than 0 and 1
 */
void NG_Fix_Update(const void *ptr)
{
	NG_arg3_u32 = (u32)ptr;	
	asm (
		"lea VRAM_RW,%a1\n	"				// a1 = VRAM_RW register address
		"move.l NG_arg3_u32,%a0\n	"		// a0 = fix update buffer

	"NG_Fix_Update_Loop:\n	"

		"move.w (%a0)+,%d0\n	"			// read first word (should be mode or stop word)
		"beq.w NG_Fix_Update_Horizontal\n	"// Horizontal update? branch
		"cmp.w #0x0001,%d0\n	"			// Vertical update?
		"beq.b NG_Fix_Update_Vertical\n	"	// branch
		"rts\n	"							// we're done
		//"bra.b NG_Fix_Update_Done\n	"		// we're done

	"NG_Fix_Update_Vertical:\n	"	
		"move.w #0x01,VRAM_MOD\n	"		// vertical update
		"bra.b NG_Fix_Update_HV_Done\n	"	// branch
	
	"NG_Fix_Update_Horizontal:\n	"
		"move.w #0x20,VRAM_MOD\n	"		// horizontal update
	
	"NG_Fix_Update_HV_Done:\n	"

		"move.w (%a0)+,%d0\n	"			// read X pos
		"asl.w #5,%d0\n	"					// shift it
		"add.w #FIXMAP+0x22,%d0\n	"		// add fix VRAM visible top left corner address
		"add.w (%a0)+,%d0\n	"				// add Y pos
		"move.w %d0,VRAM_ADDR\n	"			// set fix VRAM address

		"move.w (%a0)+,%d0\n	"			// read N (number of tiles)

	"NG_Fix_Update_Tiles_Loop:\n	"

		"move.w (%a0)+,(%a1)\n	"			// copy tile/pal data to VRAM
		"subq.w #1,%d0\n	"				// decrement N counter
		"bne.b NG_Fix_Update_Tiles_Loop\n	"// branch if N (d0) != 0
		"bra.b NG_Fix_Update_Loop\n	"		// loop

	"NG_Fix_Update_Done:\n	"

	);
}

void NG_Fix_Print_TileN_PalS(const u16 *id, u16 x, u16 y, u16 n)
{
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;
	NG_arg3_u16 = n;
	NG_arg3_u32 = (u32)id;

	asm (
		"movem.l %d2,-(%a7)\n	"
		"move.w  #0x20,VRAM_MOD\n	"


		"move.w	NG_arg1_u16,%d0\n	"
		"asl.w	#5,%d0\n	"
		"add.w  #FIXMAP+0x22,%d0\n	"
		"add.w	NG_arg2_u16,%d0\n	"
		"move.w %d0,VRAM_ADDR\n	"

		"move.l NG_arg3_u32,%a0\n	"
		"lea VRAM_RW,%a1\n	"
		"move.w NG_arg3_u16,%d0\n	"
		"subq.w #1,%d0\n	"

		"move.w	NG_PAL_ID,%d2\n	"

		"NG_Fix_Print_TileN_loop2:\n	"

			"move.w (%a0)+,%d1\n	"
			"add.w %d2,%d1\n	"
			"move.w %d1,(%a1)\n	"

		"dbra %d0,NG_Fix_Print_TileN_loop2\n	"

		"movem.l (%a7)+,%d2\n	"

		);
}

void NG_Fix_Print_TileN(const u16 *id, u16 x, u16 y, u16 n)
{
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;
	NG_arg3_u16 = n;
	NG_arg3_u32 = (u32)id;

	asm (
		"move.w  #0x20,VRAM_MOD\n	"

		"move.w	NG_arg1_u16,%d0\n	"
		"asl.w	#5,%d0\n	"
		"add.w  #FIXMAP+0x22,%d0\n	"
		"add.w	NG_arg2_u16,%d0\n	"
		"move.w %d0,VRAM_ADDR\n	"

		"move.l NG_arg3_u32,%a0\n	"
		"lea VRAM_RW,%a1\n	"
		"move.w NG_arg3_u16,%d0\n	"
		"subq.w #1,%d0\n	"

		"NG_Fix_Print_TileN_loop:\n	"

			"move.w (%a0)+,(%a1)\n	"
			NG_NOP

		"dbra %d0,NG_Fix_Print_TileN_loop\n	"

		);
}

void NG_Fix_Print_Tile(u16 id, u16 x, u16 y)
{
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;
	NG_arg3_u16 = id;

	asm (
		"move.w  #0x20,VRAM_MOD\n	"

		"move.w	NG_arg1_u16,%d0\n	"
		"asl.w	#5,%d0\n	"
		"add.w  #FIXMAP+0x22,%d0\n	"
		"add.w	NG_arg2_u16,%d0\n	"
		"move.w %d0,VRAM_ADDR\n	"

		"move.w	NG_PAL_ID,%d0\n	"
		"add.w	NG_arg3_u16,%d0\n	"
		"move.w %d0,VRAM_RW\n	"
		);
}

void NG_Fix_Print_String(const void *str, u16 x, u16 y)
{
	NG_arg1_u32 = (u32)str;
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;

	asm (
		"move.w  #0x20,VRAM_MOD\n	"

		"move.w	NG_arg1_u16,%d1\n	"
		"asl.w	#5,%d1\n	"
		"add.w  #FIXMAP+0x22,%d1\n	"
		"add.w	NG_arg2_u16,%d1\n	"
		"move.w  %d1,VRAM_ADDR\n	"

		"move.w	NG_PAL_ID,%d1\n	"
		"add.w	#FIX_ASCII,%d1\n	"


		"move.l NG_arg1_u32,%a0\n	"
		"lea VRAM_RW,%a1\n	"

		"NG_Print_Loop:\n	"
			"clr.w  %d0\n	"
			"move.b (%a0)+,%d0\n	"

			//"cmp.w  %d1,%d0\n	"
			"beq.w NG_Print_End\n	"

			"add.w	%d1,%d0\n	"
			"move.w %d0,(%a1)\n	"
		"bne.w NG_Print_Loop\n	"
		"NG_Print_End:\n	"

		);
}

void NG_Fix_Print_CPU(u16 x, u16 y)
{
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;

	asm (
		"movem.l %d2-%d4,-(%a7)\n	"
		"move.w  #0x20,VRAM_MOD\n	"

		"move.w	NG_arg1_u16,%d0\n	"
		"asl.w	#5,%d0\n	"
		"add.w  #FIXMAP+0x22,%d0\n	"
		"add.w	NG_arg2_u16,%d0\n	"
		"move.w  %d0,VRAM_ADDR\n	"

		"move.w NG_RAM+10,%d2\n	"

		"move.w	NG_PAL_ID,%d3\n	"
		"add.w	#FIX_ASCII+'0',%d3\n	"

		"lea VRAM_RW,%a0\n	"

		"move.w #100,%d4\n	"
		"move.w %d3,%d0\n	"
		"digit_convert 101\n	"
		"move.w  %d0,(%a0)\n	"

		"digit_convert10 100\n	"

		NG_NOP
		"move.w %d3,%d0\n	"
		"add.w	%d2,%d0\n	"
		"move.w %d0,(%a0)\n	"

		"move.w %d3,%d0\n	"
		"subi.w	#11,%d0\n	"
		"move.w %d0,(%a0)\n	"

		"movem.l (%a7)+,%d2-%d4\n	"
		);
}
