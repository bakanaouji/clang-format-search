#!/bin/sh
git checkout -b search
cat .gitignore_search > .gitignore
git add .gitignore
git add target_code
cd src
python hill_climbing_main.py
cd ../
git checkout .
git reset HEAD target_code
git checkout master
git reset HEAD --hard
git branch -D search
