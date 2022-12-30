#!/bin/bash
find  -type f | grep -e '.(\.C\|\.cpp\|\.cxx\|\.h)$'  | xargs -L 1 clang-format-8 -i 
echo "Clang-format has been applied over all files. Use git add -A ; git commit -m \"clang-format all files\""
