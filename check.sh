#!/bin/sh
git checkout -b check
cat .gitignore_search > .gitignore
git add .gitignore
git add target_code
git commit -m "add target_code"
find . -name "*.h" -o -name "*.hpp" -o -name "*.cpp" | xargs clang-format -i
