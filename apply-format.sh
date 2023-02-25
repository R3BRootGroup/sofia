#!/bin/bash

CLANG_FORMAT_BIN="clang-format-15"

FILES=$(find . -type f \( -iname "*.h" ! -iname "*.pb.h" ! -iname "*LinkDef.h" ! -iname "ext_h101_*.h" -o -iname "*.hpp" -o -iname "*.cxx" -o -iname "*.cpp" -o -iname "*.tpl" -o -iname "*.c" -o -iname "*.C" \) -not \( -path "./macros/*" -prune \))

$CLANG_FORMAT_BIN -i $FILES
echo "Clang-format has been applied over all files. Use git add -A ; git commit -m \"clang-format all files\""
