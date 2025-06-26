#!/bin/bash
ppu-strip build/powerpc64-ps3-elf/bin/bgdi -o build/powerpc64-ps3-elf/bin/bgdi.elf
sprxlinker build/powerpc64-ps3-elf/bin/bgdi.elf
fself build/powerpc64-ps3-elf/bin/bgdi.elf build/powerpc64-ps3-elf/bin/bgdi.self
make_self build/powerpc64-ps3-elf/bin/bgdi.elf build/powerpc64-ps3-elf/bin/bgdi.self

GAME_TITLE=GAME_TITLE
GAME_ID=GAME_ID
CONTENT_ID=UP0000-ABCDEFG_00-0000000000000000
APPVERSION=1.0

mkdir -p pkg/USRDIR
make_self_npdrm build/powerpc64-ps3-elf/bin/bgdi.elf pkg/USRDIR/EBOOT.BIN ${CONTENT_ID}
cp /usr/local/ps3dev/bin/sfo.xml .
sed -i "s/01\.00/${APPVERSION}/g" sfo.xml
sfo.py --title ${GAME_TITLE} --appid ${GAME_ID} -f /usr/local/ps3dev/bin/sfo.xml pkg/PARAM.SFO
pkg.py --contentid ${CONTENT_ID} pkg/ build/powerpc64-ps3-elf/bin/bgdi.pkg
