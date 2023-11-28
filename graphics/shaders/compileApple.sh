#!/bin/sh

COMPILER=../../External/VulkanSDK/macOS/bin/glslangValidator

DSTDIR=../../Xcode/Resources/compiledShaders


for SRC in *.vert *.frag; do

  OUT="$(echo "$SRC" | sed "s/\.frag$/-frag/" | sed "s/\.vert$/-vert/").spv"

  # don't re-compile if existing binary is newer than source file

  NEWER="$(ls -t1 "$SRC" "$OUT" | head -1)"

  if [ "$SRC" = "$NEWER" ]; then

    echo "COMPILING $OUT from:"

    $COMPILER -V "$SRC" -o "$OUT"

    # for Apple projects' benefit, copy the SPVs (vs. Xcode copying, not following, the symbolic link)

    echo "copying to $DSTDIR"

	cp "$OUT" "$DSTDIR"

  else

    echo "(unchanged $SRC)"

  fi

done
