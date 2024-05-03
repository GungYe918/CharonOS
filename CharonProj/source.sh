#!/bin/zsh

function Charon-run {
    $HOME/workspace/CharonProj/devenv/run_qemu.sh $HOME/edk2/Build/CharonLoaderX64/DEBUG_GCC/X64/Loader.efi $HOME/workspace/CharonProj/kernel/kernel.elf
}

