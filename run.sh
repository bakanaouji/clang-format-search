#!/bin/sh
git checkout -b search
cat .gitignore_search > .gitignore
git add .gitignore
git add target_code
cd src
python hill_climbing_main.py
git reset HEAD --hard
git checkout .
git checkout master
git branch -D search
