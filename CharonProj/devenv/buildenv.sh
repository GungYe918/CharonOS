# Usage: source buildenv.sh

BASEDIR="$(dirname $(realpath "$0"))/x86_64-elf"
export CPPFLAGS="-I$HOME/src/CharonOS/CharonProj/devenv/x86_64-elf/include/c++/v1 -I$BASEDIR/include/c++/v1 -I$BASEDIR/include -I$BASEDIR/include/freetype2 -nostdlibinc -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS"


export LDFLAGS="-L$BASEDIR/lib -L$HOME/src/CharonOS/CharonProj/devenv/x86_64-elf/lib"