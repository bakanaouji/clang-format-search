#!/bin/sh
git checkout -b extract
cat .gitignore_search > .gitignore
git add .gitignore
git add target_code
cd src
python extract_influential_style.py
cd ../
git reset HEAD .gitignore
git reset HEAD target_code
git checkout master
git checkout .gitignore
git checkout target_code
git branch -D extract
