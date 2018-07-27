#!/bin/sh
find $1 -name "*.h" -o -name "*.hpp" -o -name "*.cpp" | xargs clang-format -i -style="$2"