#!/bin/zsh


function qemu_charon {
    /home/gungye/workspace/CharonProj/devenv/run_qemu.sh /home/gungye/edk2/Build/CharonLoaderX64/DEBUG_GCC/X64/Loader.efi /home/gungye/workspace/CharonProj/kernel/kernel.elf
}

BASEDIR="/Users/gungye/Documents/coding/project/CharonOS/CharonProj/devenv/x86_64-elf"
export CPPFLAGS="-I$BASEDIR/include/c++/v1 -I$BASEDIR/include -I$BASEDIR/include/freetype2 -nostdlibinc -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS"
export LDFLAGS="-L$BASEDIR/lib"
