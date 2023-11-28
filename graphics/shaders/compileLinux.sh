#!/bin/sh

COMPILER=/usr/bin/glslangValidator

if [ -n "$(grep -i "name.*debian" /etc/os-release)" ]; then
  COMPILER=/usr/local/bin/glslang
fi

DSTDIR=../../CMakeLinux/build/compiledShaders

if [ ! -d $DSTDIR ]; then

  mkdir -p $DSTDIR

fi


for SRC in *.vert *.frag; do

  DSTFIL="$(echo "$SRC" | sed "s/\.frag$/-frag/" | sed "s/\.vert$/-vert/").spv"

  OUT="$DSTDIR/$DSTFIL"

  # don't re-compile if existing binary is newer than source file

  NEWER="$(ls -t1 "$SRC" "$OUT" 2>/dev/null | head -1)"

  if [ "$SRC" = "$NEWER" ]; then

    echo -n "COMPILING $OUT\n   from   "

    $COMPILER -V "$SRC" -o "$OUT"

  else

    echo "(unchanged $SRC)"

  fi

done
