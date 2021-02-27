# NGDK - Neo Geo Development Kit (beta version 0.71)

- NGDK discord server: https://discord.gg/2VCJN5c
- Neo Geo documentation: https://wiki.neogeodev.org/index.php?title=Main_Page
- More Neo Geo interesting stuff here: https://www.chibiakumas.com/68000/platform3.php

## Installation
### Windows

1. Download the NGDK archive from the Download page and unzip it where it suits to you, for instance `D:\NGDK`.
2. Download prebuilt Windows Toolchain for m68k-elf (`m68k-elf-gcc4.8.0.exe`) here: https://gnutoolchains.com/m68k-elf/
3. Download and install a Neo Geo emulator (for example http://www.kawaks.fr/).
4. Extract `m68k-elf-gcc4.8.0.exe` in `bin\m68k`.
5. Define `NGDK` environment variable with your installation path in unix path format (D:/NGDK).
6. Define `NGDK_WIN` which still point to your installation path but in windows path format (D:\NGDK).
7. Now verify everything is properly set up by trying to compile the library:

```cmd
%NGDK_WIN%\bin\make -f %NGDK_WIN%\makelib

or

%NGDK_WIN%\compile_NGDK.bat

```

You should see compilation logs then at the end you should obtain the following file

```cmd
%NGDK%\lib\libNGDK.a
```

### Linux

1. Install GCC for M68K
   1. debian/ubuntu: install gcc-8-m68k-linux-gnu package
   2. fedora: install m68k-linux-gnu package
2. Define `NGDK` environment variable with your installation path in unix path format (`/usr/NGDK`)
3. TODO: write next steps...
4. Now verify everything is properly set up by trying to compile the library:

```cmd
make -f %NGDK%/makelib
```

## About the Neo Geo...

### The Neo Geo has 6 ROMs
- C1-C2 for Sprites: automatically created when running `compile.bat --res` or `%NGDK_WIN%\bin\make -f %NGDK_WIN%\makefile ressources` in the examples/ folder.
- S1 for Fix (HuD): a 128 KiB pre-compiled file in the examples/fix/ folder.
- M1 for Z80: a pre-compiled file in the examples/sound/ folder.
- V1 for Samples: a 2 MiB blank ROM in the examples/sample/ folder.
- P1 for M68000: generated when running `compile.bat` or `%NGDK_WIN%\bin\make -f %NGDK_WIN%\makefile build` in the examples/ folder.