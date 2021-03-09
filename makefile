ROMNAME=ssideki
ROMNUM=052

BIN=$(NGDK)/bin
LIBINC=$(NGDK)/inc
LIBLIB=$(NGDK)/lib
TOOLS=$(NGDK)/bin/tools

SRC=src
OBJ=obj
OUT=out
DATA=data

MKDIR=$(BIN)/mkdir
CP=$(BIN)/cp
RM=$(BIN)/rm

RM=$(BIN)/rm

GCC_PATH=$(BIN)/gcc

CC=$(GCC_PATH)/gcc
LD=$(GCC_PATH)/ld

DEFAULT_FLAGS=-Wall -Wextra -Wno-shift-negative-value -Wno-main -Wno-unused-parameter -fno-builtin -fno-web -fno-gcse -fno-unit-at-a-time -fomit-frame-pointer -fno-inline-functions -m68000
INCS=-isystem"$(LIBINC)" -iquote"data"

SFLAGS=-B"$(GCC_PATH)" -O0 $(DEFAULT_FLAGS) $(INCS)
CFLAGS=-B"$(GCC_PATH)" -O3 $(DEFAULT_FLAGS) $(INCS)

LFLAGS=--oformat binary -Ttext -0x00 -Tbss 0x100020 -L"$(LIBLIB)"

OBJ_S=$(patsubst $(SRC)/%.s,$(OBJ)/%.o,$(wildcard $(SRC)/*.s))
OBJ_C=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(wildcard $(SRC)/*.c))

all: ressources build

build: $(OUT) $(OBJ) _build make_rom clean

_build: $(OBJ_S) $(OBJ_C)
	$(LD) $(LFLAGS) -o $(OUT)/$(ROMNUM)-p1.bin $^ -lNGDK

$(OUT):
	$(MKDIR) $(OUT)

$(OBJ):
	$(MKDIR) $(OBJ)

$(OBJ)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(SRC)/%.s
	$(CC) $(SFLAGS) -c $< -o $@

ressources:
	$(RM) $(DATA)/*.c -f
	$(RM) $(DATA)/*.h -f
	$(RM) $(DATA)/*.bin -f
	$(RM) $(DATA)/*.ktm -f
	cd $(DATA); \
	$(TOOLS)/RetroRes.exe $(TOOLS)

make_rom:
	$(CP) samples/$(ROMNUM)-v1.bin $(OUT)/$(ROMNUM)-v1.v1
	$(CP) data/$(ROMNUM)-c1.bin $(OUT)/$(ROMNUM)-c1.c1
	$(CP) data/$(ROMNUM)-c2.bin $(OUT)/$(ROMNUM)-c2.c2
	$(CP) fix/$(ROMNUM)-s1.s1 $(OUT)/$(ROMNUM)-s1.s1
	$(CP) sound/$(ROMNUM)-m1.m1 $(OUT)/$(ROMNUM)-m1.m1
	$(CP) $(OUT)/$(ROMNUM)-p1.bin $(OUT)/$(ROMNUM)-p1.p1
	$(TOOLS)/RomK.exe -invertng $(OUT)/$(ROMNUM)-p1.p1
	$(TOOLS)/RomK.exe -fillrom $(OUT)/$(ROMNUM)-p1.p1 524288
	cd $(OUT); \
	$(TOOLS)/7z.exe a -R $(ROMNAME).zip $(ROMNUM)-p1.p1 $(ROMNUM)-m1.m1 $(ROMNUM)-v1.v1 $(ROMNUM)-s1.s1 $(ROMNUM)-c1.c1 $(ROMNUM)-c2.c2

clean:
	$(RM) $(OBJ)/*.o

reset: clean
	$(RM) $(OUT)/*.*
