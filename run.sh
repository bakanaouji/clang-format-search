#!/bin/sh
git checkout -b search
cat .gitignore_search > .gitignore
git add .gitignore
git add target_code
git commit -m "add target_code"
cd src
python hill_climbing_main.py
git checkout target_code
git checkout master
git branch -D search
